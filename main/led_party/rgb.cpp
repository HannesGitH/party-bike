#include "rgb.h"

struct led_color_t add_colors(struct led_color_t a,struct led_color_t b){
    struct led_color_t new_color = {
        (uint8_t)(b.red + a.red),
        (uint8_t)(b.green + a.green),
        (uint8_t)(b.blue + a.blue)
    };
    return new_color;
}