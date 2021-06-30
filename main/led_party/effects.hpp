#include <stddef.h>
#include "led_strip/led_strip.h"
#include "partyman.hpp"
#ifndef EFFECTS_H
#ifdef __cplusplus
extern "C" {
#endif
#define EFFECTS_H

struct effect{
    uint32_t repitions;
    void (*draw)(led_strip_t * strips, uint32_t step);
};

void drive_effect(led_strip_t * strips,uint step_millis, struct effect effect);

// void effect_spread_pixel_draw(led_strip_t * strips, uint32_t step);
// void effect_walk_pixel_draw(led_strip_t * strips, uint32_t step);

extern effect effect_walk_pixel;
extern effect effect_spread_pixel;


#ifdef __cplusplus
}
#endif
#endif