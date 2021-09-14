#include <stddef.h>
#include "led_strip.h"
#include "stripdata.hpp"
#ifndef EFFECTS_H
#ifdef __cplusplus
extern "C" {
#endif
#define EFFECTS_H

struct effect{
    uint32_t repetitions;
    void (*draw)(led_strip_t * strips, uint32_t step, void* extra_args_p);
};

//todo : the null default doesnt really work
void drive_effect(led_strip_t * strips,uint step_millis, struct effect effect, void* extra_args_p = NULL);
void drive_effects(led_strip_t * strips,uint step_millis, struct effect * effects, uint8_t amount, void * extra_args_p[] = NULL, bool run_all_in_every_step = false);

// void effect_spread_pixel_draw(led_strip_t * strips, uint32_t step);
// void effect_walk_pixel_draw(led_strip_t * strips, uint32_t step);

extern effect effect_walk_pixel;
extern effect effect_spread_pixel;
extern effect effect_walking_colorline;
extern effect effect_streetlight;
extern effect effect_init_rainbow;
extern effect effect_change_hue;

struct effect effects[] = {
    effect_walk_pixel,
    effect_walking_colorline,
    effect_init_rainbow,
    effect_change_hue,
    effect_streetlight
};


#ifdef __cplusplus
}
#endif
#endif