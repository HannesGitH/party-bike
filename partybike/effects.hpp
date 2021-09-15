#include <stddef.h>
#include "led_strip.h"
#include "stripdata.hpp"
#ifndef EFFECTS_H
#ifdef __cplusplus
extern "C" {
#endif
#define EFFECTS_H

struct Effect{
    uint32_t repetitions;
    void (*draw)(led_strip_t * strips, uint32_t step, void* extra_args_p);
};

//todo : the null default doesnt really work
void drive_effect(led_strip_t * strips,uint step_millis, struct Effect effect, void* extra_args_p = NULL);
void drive_effects(led_strip_t * strips,uint step_millis, struct Effect * effects, uint8_t amount, void * extra_args_p[] = NULL, bool run_all_in_every_step = false);

// void effect_spread_pixel_draw(led_strip_t * strips, uint32_t step);
// void effect_walk_pixel_draw(led_strip_t * strips, uint32_t step);

extern Effect effect_walk_pixel;
extern Effect effect_spread_pixel;
extern Effect effect_walking_colorline;
extern Effect effect_streetlight;
extern Effect effect_init_rainbow;
extern Effect effect_change_hue;
extern Effect effect_set_color;

//Effect effect_possibilities[6]; //TODO make the effects iterateable?


#ifdef __cplusplus
}
#endif
#endif