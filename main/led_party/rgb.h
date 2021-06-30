
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#ifndef RGB_H
#ifdef __cplusplus
extern "C" {
#endif
#define RGB_H

/**
 * RGB LED colors
 * first 8 bit for flags last 3 bytes for r g b respectively
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

//todo
irgb_t lighten_fast(irgb_t iRGB, int8_t amount);
irgb_t lighten(irgb_t iRGB, int8_t amount);

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