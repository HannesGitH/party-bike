#include "api.hpp"

BluetoothSerial serialBT;

bool locked = true;

Api api("Partybike");

void setup() {
}

void loop() {
  api.run();
  vTaskDelay(25/portTICK_PERIOD_MS);
}