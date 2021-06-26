#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "led_party_man.hpp"
#include "rgb.h"

#ifndef LEDPARTY_H
#define LEDPARTY_H

void led_party_task(void *arg);

#endif