
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#ifndef RGB_H
#ifdef __cplusplus
extern "C" {
#endif
#define RGB_H

/**
 * RGB LED colors
 */
typedef uint32_t irgb_t;

irgb_t iRGB
(
    uint8_t R,
    uint8_t G,
    uint8_t B
);
struct led_color_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

// TODO make this overloaded operator+()
struct led_color_t add_colors(struct led_color_t a,struct led_color_t b);
#ifdef __cplusplus
}
#endif
#endif