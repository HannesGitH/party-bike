
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

irgb_t invert(irgb_t iRGB);

#ifdef __cplusplus
}
#endif
#endif