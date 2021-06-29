#include "rgb.h"

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
