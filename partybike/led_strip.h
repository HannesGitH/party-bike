#ifndef LED_STRIP_H
#define LED_STRIP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/rmt.h>
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "rgb.h"

#include <stddef.h>

struct led_strip_t {
    uint32_t led_strip_length;

    gpio_num_t gpio; // Must be less than GPIO_NUM_33

    // RMT peripheral settings
    rmt_channel_t rmt_channel;

    /*
     * Interrupt table is located in soc.h
     * As of 11/27/16, reccomended interrupts are:
     * 9, 12, 13, 17, 18, 19, 20, 21 or 23
     * Ensure that the same interrupt number isn't used twice
     * across all libraries
     */
    int rmt_interrupt_num;

    irgb_t * led_strip_buf;
    bool is_dirty;

    xSemaphoreHandle access_semaphore;
};

bool led_strip_init(struct led_strip_t *led_strip);

/**
 * Sets the pixel at pixel_num to color.
 */
bool led_strip_set_pixel_color(struct led_strip_t *led_strip, uint32_t pixel_num, irgb_t color);
bool led_strip_set_pixel_rgb(struct led_strip_t *led_strip, uint32_t pixel_num, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Get the pixel color at pixel_num for the led strip that is currently being shown! 
 * NOTE: If you call set_pixel_color then get_pixel_color for the same pixel_num, you will not 
 * get back the same pixel value. This gets you the color of the pixel currently being shown, not the one
 * being updated
 *
 * If there is an invalid argument, color will point to NULL and this function will return false.
 */
bool led_strip_get_pixel_color(struct led_strip_t *led_strip, uint32_t pixel_num, irgb_t *color);

bool led_strip_addto_pixel_color(struct led_strip_t *led_strip, uint32_t pixel_num, irgb_t color);

/**
 * Clears the LED strip.
 */
bool led_strip_clear(struct led_strip_t *led_strip);

#ifdef __cplusplus
}
#endif

#endif // LED_STRIP_H
