
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

enum RGB_dim{
    red=0,
    green=1,
    blue=2
};

irgb_t invert(irgb_t iRGB);

uint8_t dim_from_irgb(irgb_t irgb, enum RGB_dim dim);
irgb_t change_hue(irgb_t irgb, float H);

irgb_t change_hsv_wrong(
    irgb_t in,        // color to transform
    float h,          // hue shift (in degrees)
    float s,          // saturation multiplier (scalar)
    float v           // value multiplier (scalar)
);

irgb_t change_hue(
    irgb_t in,        // color to transform
    float h          // hue shift (in degrees)
);

#ifdef __cplusplus
}
#endif
#endif