#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "../led_strip/led_strip.h"
#ifndef LEDPARTY_H
#define LEDPARTY_H

void led_party_task(void *arg);

class Led_party_man
{
private:
    /* data */
public:
    Led_party_man(gpio_num_t data_pin);
    ~Led_party_man();
};


#endif