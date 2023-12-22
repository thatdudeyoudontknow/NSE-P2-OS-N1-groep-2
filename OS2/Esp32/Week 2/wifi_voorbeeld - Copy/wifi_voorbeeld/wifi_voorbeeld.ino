#include <WiFi.h>
#include "secret.h"
#include "time.h"
#include "esp_sntp.h"

//constants for SNTP client
const char* ntpServer = "nl.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

//global variables for timestamp
const int glob_buf_size = (64* sizeof(char));
char *glob_time_buf;

//conect with NTP server
void SNTP_connect(){
  const time_t old_past = 1577836800; // 2020-01=01T00:00:00Z
  printf("\r\nConnect to SNTP server\n");
  //init time protocol sync
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();
  //https://knowledgebase.progress.com/articles/article/P129473
  setenv("TZ", "CET-1CEST-2, m3.5.0/02:00:00, M10.5.0/03:00:00", 1 );
  tzset();

  bool connected =false ;
  while (!connected) {
    delay(500);
    time_t now;
    if (time(&now) <old_past) {//if there isno connection with NTP server then local time returns Unix epoch (1970)
      printf(".");
      }
    else { 
      printf("CONNECTED\r\n");
      connected = true;
    }
  }
}

// get cyurrent time
//https://docs.espressif.com/projects/esp=idf/en/latest/esp32/api-reference/system/system_time.html
void getLocalTime(char*time_buf, int time_buf_size){
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(time_buf, time_buf_size, "%c", &timeinfo);
  return;
}


// data below is set as a define in the file secret.h
//const char* ssid = "HANZE-ZP11";
//const char* password = "dby555YcV2";
void WiFi_connect(){
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  // Print timestamp
  Serial.print(millis());
  Serial.print(": ");

  // Print "Connected" message on a new line
  Serial.println("Connected");
  }
  Serial.print("Connected to WiFi: ");
  Serial.println(WiFi.SSID());
  }
void setup(){
  Serial.begin(115200);
  WiFi_connect();
  SNTP_connect();
  //allocate heap memory to store 1 timestamp
  glob_time_buf = (char*)malloc(glob_buf_size);
  //check if memory is allocated
  assert(glob_time_buf != NULL);
}

void loop(){
  getLocalTime(glob_time_buf, glob_buf_size);
  printf( "Current Time: %s\r\n", glob_time_buf);
  delay(5000);

}