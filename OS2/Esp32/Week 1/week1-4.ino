

#define LED1 25  
#define LED2 26  
#define LED3 27

struct s_led {
  byte gpio;           
  byte state;          
  unsigned napms;     
  TaskHandle_t taskh;  
};

static s_led leds[3] = {
  { LED1, 0, 500, 0 },
  { LED2, 0, 200, 0 },
  { LED3, 0, 750, 0 }
};

static void led_task_func(void *argp) {
  s_led *ledp = (s_led *)argp;
  unsigned stack_hwm = 0, temp;

  delay(1000);

  for (;;) {
    digitalWrite(ledp->gpio, ledp->state ^= 1);
    temp = uxTaskGetStackHighWaterMark(nullptr);
    if (!stack_hwm || temp < stack_hwm) {
      stack_hwm = temp;
      printf("Task for gpio %d has stack hwm %u\n",
             ledp->gpio, stack_hwm);
    }
    delay(ledp->napms);
  }
}

void setup() {
  int app_cpu = 0;  

  delay(500);  

  app_cpu = xPortGetCoreID();
  printf("app_cpu is %d (%s core)\n",
         app_cpu,
         app_cpu > 0 ? "Dual" : "Single");

  printf("LEDs on gpios: ");
  for (auto &led : leds) {
    pinMode(led.gpio, OUTPUT);
    digitalWrite(led.gpio, LOW);
    xTaskCreatePinnedToCore(
      led_task_func,
      "led_task",
      2048,
      &led,
      1,
      &led.taskh,
      app_cpu);
    printf("%d ", led.gpio);
  }
  putchar('\n');
}



void loop() {
  
  delay(10000);
  
  vTaskSuspend(leds[0].taskh);
 
  delay(3000);
  
  vTaskSuspend(leds[1].taskh);
  
  delay(3000);
 
  vTaskSuspend(leds[2].taskh);
  
  delay(3000);
 
  vTaskResume(leds[0].taskh);
  vTaskResume(leds[1].taskh);
  vTaskResume(leds[2].taskh);

}