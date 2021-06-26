#include "rgb.h"

irgb_t iRGB
(
    uint8_t R,
    uint8_t G,
    uint8_t B
){
    return (uint32_t)R<<16 | (uint32_t)G<<8 | (uint32_t)B;
}

struct led_color_t add_colors(struct led_color_t a,struct led_color_t b){
    struct led_color_t new_color = {
        (uint8_t)(b.red + a.red),
        (uint8_t)(b.green + a.green),
        (uint8_t)(b.blue + a.blue)
    };
    return new_color;
}