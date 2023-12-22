// press2.ino
// MIT License (see file LICENSE)
#include <WiFi.h>
#include "secret.h"
#include "time.h"
#include "esp_sntp.h"

#define GPIO_LED 27
#define GPIO_BUTTONL 25
#define GPIO_BUTTONR 26

const char* ntpServer = "nl.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

const int glob_buf_size = (64 * sizeof(char));
char *glob_time_buf;

void SNTP_connect() {
  const time_t old_past = 1577836800; // 2020-01=01T00:00:00Z
  Serial.println("\r\nConnect to SNTP server");
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();
  setenv("TZ", "CET-1CEST-2, m3.5.0/02:00:00, M10.5.0/03:00:00", 1);
  tzset();

  bool connected = false;
  while (!connected) {
    delay(500);
    time_t now;
    if (time(&now) < old_past) {
      Serial.print(".");
    } else {
      Serial.println("CONNECTED");
      connected = true;
    }
  }
}

void getLocalTime(char *time_buf, int time_buf_size) {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(time_buf, time_buf_size, "%c", &timeinfo);
  return;
}

void WiFi_connect() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi: ");
  Serial.println(WiFi.SSID());
}

static QueueHandle_t queue;
static QueueHandle_t audit_queue;
static const int max_audit_entries = 20;
static int num_audit_entries = 0;

void addAuditEntryAndPrint() {
  if (num_audit_entries < max_audit_entries) {
    getLocalTime(glob_time_buf, glob_buf_size);
    xQueueSendToBack(audit_queue, glob_time_buf, 0);
    num_audit_entries++;
    Serial.print("Button pressed at: ");
    Serial.println(glob_time_buf);
    Serial.println(num_audit_entries);
  } else {
    char old_entry[glob_buf_size];
    xQueueReceive(audit_queue, &old_entry, 0);
    getLocalTime(glob_time_buf, glob_buf_size);
    xQueueSendToBack(audit_queue, glob_time_buf, 0);
    Serial.print("Button pressed at: ");
    Serial.println(glob_time_buf);
    // Print the contents of the audit queue
    for (int i = 0; i < num_audit_entries; ++i) {
        char entry[glob_buf_size];
        xQueueReceive(audit_queue, &entry, 0);
        Serial.print("Audit entry ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(entry);
    }
  }
}

static const int reset_press = -998;

static void debounce_task(void *argp) {
  unsigned button_gpio = *(unsigned *)argp;
  uint32_t level, state = 0;
  uint32_t mask = 0x7FFFFFFF;
  int event, last = -999;

  for (;;) {
    level = !digitalRead(button_gpio);
    state = (state << 1) | level;
    if ((state & mask) == mask)
      event = button_gpio;  // Press
    else
      event = -button_gpio; // Release

    if (event != last) {
      if (xQueueSendToBack(queue, &event, 0) == pdPASS) {
        last = event;
      } else if (event < 0) {
        do {
          xQueueReset(queue);
        } while (xQueueSendToBack(queue, &reset_press, 0) != pdPASS);
        last = event;
      }
    }
    taskYIELD();
  }
}

static void press_task(void *argp) {
  static const uint32_t enable = (1 << GPIO_BUTTONL) | (1 << GPIO_BUTTONR);
  BaseType_t s;
  int event;
  uint32_t state = 0;

  digitalWrite(GPIO_LED, LOW);

  for (;;) {
    s = xQueueReceive(
        queue,
        &event,
        portMAX_DELAY);
    assert(s == pdPASS);

    if (event == reset_press) {
      digitalWrite(GPIO_LED, LOW);
      state = 0;
      printf("RESET!!\n");
      continue;
    }

    if (event >= 0) {
      state |= 1 << event;
      if (state == enable) {
        addAuditEntryAndPrint();
        digitalWrite(GPIO_LED, HIGH);
      }
    } else {
      state &= ~(1 << -event);
      if (state != enable) {
        digitalWrite(GPIO_LED, LOW);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi_connect();
  SNTP_connect();
  glob_time_buf = (char *)malloc(glob_buf_size);
  assert(glob_time_buf != NULL);

  int app_cpu = xPortGetCoreID();
  static int left = GPIO_BUTTONL;
  static int right = GPIO_BUTTONR;
  TaskHandle_t h;
  BaseType_t rc;

  delay(2000);
  queue = xQueueCreate(2, sizeof(int));
  assert(queue);

  audit_queue = xQueueCreate(max_audit_entries, glob_buf_size);
  assert(audit_queue);

  pinMode(GPIO_LED, OUTPUT);
  pinMode(GPIO_BUTTONL, INPUT_PULLUP);
  pinMode(GPIO_BUTTONR, INPUT_PULLUP);

  rc = xTaskCreatePinnedToCore(
      debounce_task,
      "debounceL",
      2048,
      &left,
      1,
      &h,
      app_cpu);
  assert(rc == pdPASS);
  assert(h);

  rc = xTaskCreatePinnedToCore(
      debounce_task,
      "debounceR",
      2048,
      &right,
      1,
      &h,
      app_cpu);
  assert(rc == pdPASS);
  assert(h);

  rc = xTaskCreatePinnedToCore(
      press_task,
      "led",
      2048,
      nullptr,
      1,
      &h,
      app_cpu);
  assert(rc == pdPASS);
  assert(h);
}

void loop() {
  // Loop is not used in this example
}
