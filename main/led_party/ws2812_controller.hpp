#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "../led_strip/led_strip.h"
#include "rgb.h"

#ifndef WS2812_H
#define WS2812_H

class Ws2812_controller
{
private:
    xSemaphoreHandle sema;
    irgb_t *led_strip_buf_1;
    struct led_strip_t led_strip;
    
    irgb_t * temporary_pixel;
    
public:
    Ws2812_controller(gpio_num_t strip_data_pin, uint length, gpio_num_t high_v_pin);
    ~Ws2812_controller();

    void update();
    void reset();
    void set_color(uint32_t index, irgb_t iRGB);
    irgb_t get_color(uint32_t index);
    void change_color(uint32_t index, irgb_t iRGB);
};

#endif