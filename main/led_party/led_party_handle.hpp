#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "ws2812_controller.hpp"
#include "rgb.h"

#ifndef LEDPARTY_H
#define LEDPARTY_H

void led_party_task(void *arg);

#endif