#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/timer.h"
#include "hal/timer_types.h"

#define TGROUP TIMER_GROUP_1
#define TTIMER TIMER_1


class Delayer
{
private:
    uint64_t getTicks();
public:
    Delayer(/* args */);
    ~Delayer();
    void delay(uint32_t ticks);
};


class Ws2812
{
private:
    gpio_num_t data_pin;
    Delayer d;

public:
    Ws2812(gpio_num_t data_pin);
    ~Ws2812();
};

