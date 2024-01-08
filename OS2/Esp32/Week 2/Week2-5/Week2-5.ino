#include "time.h"

// LED is active high
#define GPIO_LED_SEM 14 
#define GPIO_BUTTONL  27
#define GPIO_BUTTONR  26

#define DEBOUNCE_TIME  30 // the debounce time in milliseconds, increase this time if it still chatters

// Declare SemaphoreHandle
SemaphoreHandle_t sem;

//
// Button Debouncing task:
//
static void debounce_task(void *argp) {

  unsigned button_gpio = *(unsigned*)argp;
  // Variables will change:
  int lastSteadyState = LOW;       // the previous steady state from the input pin
  int lastFlickerableState = LOW;  // the previous flickerable state from the input pin
  int currentState;                // the current reading from the input pin

  // the following variables are unsigned longs because the time, measured in
  // milliseconds, will quickly become a bigger number than can be stored in an int.
  //https://esp32io.com/tutorials/esp32-button-debounce
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  
  for (;;) {

    currentState = digitalRead(button_gpio);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch/button changed, due to noise or pressing:
    if (currentState != lastFlickerableState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
      // save the last flickerable state
      lastFlickerableState = currentState;
    }
    if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:
      // if the button state has changed:
      if(lastSteadyState == HIGH && currentState == LOW){
        printf("The button %d is pressed\r\n", button_gpio);
        if (button_gpio==26)
          blink_sem_led(200);
        else
          blink_sem_led(400);
      }
      else if(lastSteadyState == LOW && currentState == HIGH){
        printf("The button %d is released\r\n", button_gpio);
      }
      // save the last steady state
      lastSteadyState = currentState;
    }
    taskYIELD();
  }
}

// Function to blink GPIO_led_sem at a given rate (ms) for 10 times 
void blink_sem_led(int rate){

  // Set Semaphore and release the Semaphore when done
  if (xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE) {
    int count = 0;
    printf("BLINK_SEM_LED, rate: %d\r\n", rate);

    while (count<10){
      digitalWrite(GPIO_LED_SEM, HIGH);
      delay(rate);
      digitalWrite(GPIO_LED_SEM, LOW);
      delay(rate);
      count++;
    }

    // Free Semaphore
    xSemaphoreGive(sem);
  }
}

// Initialization:
void setup() {
  int app_cpu = xPortGetCoreID();
  static int left = GPIO_BUTTONL;
  static int right = GPIO_BUTTONR;
  TaskHandle_t h;
  BaseType_t rc;
  
  delay(2000);          // Allow USB to connect

  // Create a binary Semaphore and release the Semaphore
  sem = xSemaphoreCreateBinary();
  assert(sem);
  xSemaphoreGive(sem);

  pinMode(GPIO_LED_SEM, OUTPUT);
  pinMode(GPIO_BUTTONL, INPUT_PULLUP);
  pinMode(GPIO_BUTTONR, INPUT_PULLUP);

  rc = xTaskCreatePinnedToCore(
    debounce_task,
    "debounceL",
    2048,     // Stack size
    &left,    // Left button gpio
    1,        // Priority
    &h,       // Task handle
    app_cpu   // CPU
  );
  assert(rc == pdPASS);
  assert(h);

  rc = xTaskCreatePinnedToCore(
    debounce_task,
    "debounceR",
    2048,     // Stack size
    &right,   // Right button gpio
    1,        // Priority
    &h,       // Task handle
    app_cpu   // CPU
  );
  assert(rc == pdPASS);
  assert(h);
}

// Not used
void loop() {
  vTaskDelete(nullptr);
}
