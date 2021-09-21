
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#ifndef RGB_H
#ifdef __cplusplus
extern "C" {
#endif
#define RGB_H

struct irgb_t
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t i; //left/right/centre first/second/third bit;  0xFF = end of custom; 0xFE = end of buffer; 0xFD = skip this  
    
    irgb_t& operator +(const irgb_t& a)
    {
        r += a.r;
        g += a.g;
        b += a.b;    
        return *this;
    }
    irgb_t& operator +=(const irgb_t& a){return *this+a;}

    irgb_t& operator *(const irgb_t& a)
    {
        r *= a.r;
        g *= a.g;
        b *= a.b;    
        return *this;
    }
};

extern irgb_t black;
extern irgb_t white;

irgb_t fromarray(uint8_t v4[4]);

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

irgb_t lighten_fast(irgb_t iRGB, int8_t amount);


irgb_t change_hsv(
    irgb_t in, 
    const float fHue,
    const float fSat,
    const float fVal
);

irgb_t change_hue(
    irgb_t in,        // color to transform
    float h          // hue shift (in degrees)
);

#ifdef __cplusplus
}
#endif
#endif