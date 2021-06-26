#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "../led_strip/led_strip.h"
#include "rgb.h"

#ifndef LEDPARTYMAN_H
#define LEDPARTYMAN_H

class Led_party_man
{
private:
    xSemaphoreHandle sema;
    struct led_color_t *led_strip_buf_1;
    struct led_color_t *led_strip_buf_2;
    struct led_strip_t led_strip;
    
    led_color_t * temporary_pixel;
    
public:
    Led_party_man(gpio_num_t strip_data_pin, uint length, gpio_num_t high_v_pin);
    ~Led_party_man();

    void update();
    void set_color(uint32_t index, led_color_t RGB);
    led_color_t get_color(uint32_t index);
    void change_color(uint32_t index, led_color_t RGB);
};

#endif