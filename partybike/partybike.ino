#include "api.hpp"
#include <Arduino.h>
//#include "freertos/FreeRTOS.h"

void mainTask(void * params){
  Api api("Partybike");
  for(;;){
    api.run();
    vTaskDelay(25/portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreate(
    mainTask,
    "api main task",
    8192,
    NULL,
    1,
    NULL //taskhandle not needed rn
  );
}

void loop() {
    Serial.println("im still alive!");
    vTaskDelay(25000/portTICK_PERIOD_MS);
}