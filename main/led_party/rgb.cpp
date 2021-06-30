#include "rgb.h"
#include "math.h"

irgb_t iRGB
(
    uint8_t R,
    uint8_t G,
    uint8_t B
){
    return (uint32_t)R<<16 | (uint32_t)G<<8 | (uint32_t)B;
}

irgb_t invert(irgb_t iRGB){
    return (iRGB ^ 0x00FFFFFF)+1;
}

struct Color{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

irgb_t irgb_from_color
(
    Color c
){
    return iRGB(c.r,c.g,c.b);
}


uint8_t dim_from_irgb(irgb_t irgb, RGB_dim dim){
    return (irgb>>(8*(2-dim)));
}

irgb_t change_hue(
    irgb_t in,
    float H
){
    float U = cos(H*M_PI/180);
    float W = sin(H*M_PI/180);

    Color C;
    C.r = (.299+.701*U+.168*W)*dim_from_irgb(in,red)
        + (.587-.587*U+.330*W)*dim_from_irgb(in,green)
        + (.114-.114*U-.497*W)*dim_from_irgb(in,blue);
    C.g = (.299-.299*U-.328*W)*dim_from_irgb(in,red)
        + (.587+.413*U+.035*W)*dim_from_irgb(in,green)
        + (.114-.114*U+.292*W)*dim_from_irgb(in,blue);
    C.b = (.299-.3*U+1.25*W)*dim_from_irgb(in,red)
        + (.587-.588*U-1.05*W)*dim_from_irgb(in,green)
        + (.114+.886*U-.203*W)*dim_from_irgb(in,blue);
    return irgb_from_color(C);
}