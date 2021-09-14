#include "effects.hpp"
#include "stripdata.hpp"
#include "../misc/math.hpp"

void drive_effect(led_strip_t * strips,uint step_millis, struct effect effect, void * extra_args_p){
    for(int step=0;step<effect.repetitions;step++){
        effect.draw(strips,step,extra_args_p);
        vTaskDelay(step_millis / portTICK_PERIOD_MS);
    }
}

void drive_effects(led_strip_t * strips,uint step_millis, effect * effects, uint8_t amount, void * extra_args_p[], bool run_all_in_every_step){
    uint32_t max_reps = 0;
    for (uint16_t i = 0; i < amount; i++)
    {
        max_reps = max(max_reps,effects[i].repetitions);
    }
    
    for(int step=0;step<max_reps;step++){
        for (uint16_t i = 0; i < amount; i++)
        {
            if(run_all_in_every_step || effects[i].repetitions < step)
            //printf("%d effect argument %d points to %d\n",i,(int)extra_args_p[i],*(uint8_t*) extra_args_p[i]);
            effects[i].draw(strips,step, extra_args_p ? extra_args_p[i] : NULL);
        }
        vTaskDelay(step_millis / portTICK_PERIOD_MS);
    }
}

void effect_spread_pixel_draw(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = {.r = 0xFF,.g = 0x44,.b = 0xCC,};
    for(uint i=0; i<amount_strips; i++){
        if(step<=strip_lengths[i]){
            led_strip_addto_pixel_color(strips+i,step,walking_color);
            led_strip_addto_pixel_color(strips+i,step-1,invert(walking_color));
        }
    }
}

void draw_iterated_pixel(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = extra_args_p ? *(irgb_t*) extra_args_p : (irgb_t){.r = 0xFF,.g = 0x44,.b = 0xCC,};

    if(step<LENGTH_MAIN_L){
        //main_r
        led_strip_addto_pixel_color(strips+MAIN,step,walking_color);
        //main_l
        led_strip_addto_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T-step-1,walking_color);
        //rear_t
        led_strip_addto_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T+step,walking_color);
    }

    if (LENGTH_MAIN_L<step&&step<(LENGTH_MAIN_L+LENGTH_DIAG_L+1))
    {
        uint32_t step2=step-LENGTH_MAIN_L;
        //diag_r
        led_strip_addto_pixel_color(strips+DIAG,LENGTH_DIAG_R-step2,walking_color);
        //diag_l
        led_strip_addto_pixel_color(strips+DIAG,LENGTH_DIAG_R+step2-1,walking_color);
        //frnt
        led_strip_addto_pixel_color(strips+FRNT,step2+3,walking_color);
    }  
    
    if (LENGTH_MAIN_L+LENGTH_DIAG_L<step&&step<LENGTH_MAIN_L+LENGTH_DIAG_L+LENGTH_SDDL)
    {
        uint32_t step3=step-(LENGTH_MAIN_L+LENGTH_DIAG_L);
        //sddl
        led_strip_addto_pixel_color(strips+SDDL,LENGTH_SDDL-step3,walking_color);
        //rear_b
        led_strip_addto_pixel_color(strips+REAR,LENGTH_REAR_B-step3,walking_color);
    }

}


void remove_iterated_pixel(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = extra_args_p ? *(irgb_t*) extra_args_p : (irgb_t){.r = 0xFF,.g = 0x44,.b = 0xCC,};

    if(0<step&&step<LENGTH_MAIN_L){
        //undo
        //main_r
        led_strip_addto_pixel_color(strips+MAIN,step-1,invert(walking_color));
        //main_l
        led_strip_addto_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T-step,invert(walking_color));
        //rear_t
        led_strip_addto_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T+step-1,invert(walking_color));    
    }
 
    if(LENGTH_MAIN_L<(step)&&(step)<(LENGTH_MAIN_L+LENGTH_DIAG_L+1)){//undo
        uint32_t step2=step-LENGTH_MAIN_L;
        //diag_r
        led_strip_addto_pixel_color(strips+DIAG,LENGTH_DIAG_L-step2,invert(walking_color));
        //diag_l
        led_strip_addto_pixel_color(strips+DIAG,LENGTH_DIAG_R+step2-1,invert(walking_color));
        //frnt
        led_strip_addto_pixel_color(strips+FRNT,step2+3,invert(walking_color));
    }
    
    if (LENGTH_MAIN_L+LENGTH_DIAG_L<(step)&&(step)<LENGTH_MAIN_L+LENGTH_DIAG_L+LENGTH_SDDL){//undo
        uint32_t step3=step-(LENGTH_MAIN_L+LENGTH_DIAG_L);
        //sddl
        led_strip_addto_pixel_color(strips+SDDL,LENGTH_SDDL-step3,invert(walking_color));
        //rear_b
        led_strip_addto_pixel_color(strips+REAR,LENGTH_REAR_B-step3,invert(walking_color));
    }
}

void effect_walk_pixel_draw(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = extra_args_p ? *(irgb_t*) extra_args_p : (irgb_t){.r = 0xFF,.g = 0x44,.b = 0xCC,};
    draw_iterated_pixel(strips, step, extra_args_p);
    remove_iterated_pixel(strips, step, extra_args_p);
}

effect effect_walk_pixel{
    .repetitions = LENGTH_MAIN_L+LENGTH_SDDL+LENGTH_DIAG_L+1,
    .draw = effect_walk_pixel_draw
};
effect effect_spread_pixel{
    .repetitions = LENGTH_MAIN,
    .draw = effect_spread_pixel_draw  
};

void effect_walking_colorline_draw(led_strip_t * strips, uint32_t step, void* extra_args_p){
    uint8_t length = extra_args_p ? *(uint8_t*) extra_args_p : 10;
    irgb_t color = (irgb_t){.r = 0xFF,.g = 0x44,.b = 0xCC,};
    for (uint8_t i = 0; i < length; i++)
    {
        float sat = (float)(length-i)/(float)length;
        irgb_t current_color = change_hsv(color,(float)i*10,sat,sat);
        effect_walk_pixel.draw(strips,step-i,&current_color);
    }
}

effect effect_walking_colorline{
    .repetitions = LENGTH_MAIN_L+LENGTH_SDDL+LENGTH_DIAG_L+1,
    .draw = effect_walking_colorline_draw
};


void effect_streetlight_draw(led_strip_t * strips, uint32_t step, void* bool__turn_off_everything_else_so_its_legal){
    //todo why and when does this crash (in drive_effects if extraagrmunt is NULL) but when else and most importantly - WHY?!
    bool legal = false;//bool__turn_off_everything_else_so_its_legal ? *(bool*) bool__turn_off_everything_else_so_its_legal : false; //yo we just wanna turn the other running effects off if we reeeally wanna be legal
    
    if(legal){
        for(int i=0; i<amount_strips; i++){
            led_strip_clear(strips+i);
        }
    }

    //turn rear_b red
    for (uint8_t i = 0; i < strip_lengths[REAR]; i++)
    {
        led_strip_set_pixel_color(strips+REAR,i,(irgb_t){.r = 0xFF,.g = 0x00,.b = 0x00,});
    }
    //turn rear_t red as well
    for (uint8_t i = 0; i < LENGTH_REAR_T; i++)
    {
        led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN-LENGTH_REAR_T+i,(irgb_t){.r = 0xFF,.g = 0x00,.b = 0x00,});
    }

    //turn front white
    for (uint8_t i = 0; i < strip_lengths[FRNT]-3; i++)//there r 3 pixels not facing the front on the frnt strip
    {
        led_strip_set_pixel_color(strips+FRNT,i,(irgb_t){.r = 0xFF,.g = 0xFF,.b = 0xFF,});
    }
    /* 
    //saddle is also facing the front so turn it white as well
    for (uint8_t i = 0; i < strip_lengths[SDDL]; i++)
    {
        led_strip_set_pixel_color(strips+SDDL,i,0xFFFFFF);
    }
     */
    //turn other pixels facing the front white
    led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN_R  ,(irgb_t){.r = 0xFF,.g = 0xFF,.b = 0xFF,});
    led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN_R-1,(irgb_t){.r = 0xFF,.g = 0xFF,.b = 0xFF,});
    led_strip_set_pixel_color(strips+DIAG,LENGTH_DIAG_R  ,(irgb_t){.r = 0xFF,.g = 0xFF,.b = 0xFF,});
    led_strip_set_pixel_color(strips+DIAG,LENGTH_DIAG_R-1,(irgb_t){.r = 0xFF,.g = 0xFF,.b = 0xFF,});
    
    //turn other pixels facing the back red
    led_strip_set_pixel_color(strips+FRNT,LENGTH_FRNT-1,(irgb_t){.r = 0xFF,.g = 0x00,.b = 0x00,});
    led_strip_set_pixel_color(strips+FRNT,LENGTH_FRNT-2,(irgb_t){.r = 0xFF,.g = 0x00,.b = 0x00,});
    led_strip_set_pixel_color(strips+FRNT,LENGTH_FRNT-3,(irgb_t){.r = 0xFF,.g = 0x00,.b = 0x00,});
}

effect effect_streetlight{
    .repetitions = 1,
    .draw = effect_streetlight_draw
};


void effect_init_rainbow_draw(led_strip_t * strips, uint32_t step, void* total_hue_rotations_p){
    int total_hue_rotations = 2;//total_hue_rotations_p ? *(int*) total_hue_rotations_p : 1;
    uint8_t pixels = LENGTH_MAIN_L+LENGTH_DIAG_L+LENGTH_SDDL;
    float hue_change = fmod(360*(((float)total_hue_rotations)/pixels),360);
    irgb_t color = (irgb_t){.r = 0x00,.g = 0x00,.b = 0xFE,};
    for (uint8_t i = 0; i < pixels; i++)
    {
        irgb_t current_color = change_hue(color,i*hue_change);
        draw_iterated_pixel(strips,i,&current_color);
    }
}

effect effect_init_rainbow{
    .repetitions = 1,
    .draw = effect_init_rainbow_draw
};

void effect_change_hue_draw(led_strip_t * strips, uint32_t step, void* hue_rotation_p){
    float hue_change = 10;//hue_rotation_p ? fmod(*(float*) hue_rotation_p,360) : 1;
    irgb_t color;
    for(int i=0; i<amount_strips; i++){
        for (uint16_t j = 0; j < strip_lengths[i]; j++)
        {
            led_strip_get_pixel_color(strips+i,j,&color);
            led_strip_set_pixel_color(strips+i,j,change_hue(color,hue_change));
        }
    }
}

effect effect_change_hue{
    .repetitions = 360,
    .draw = effect_change_hue_draw
};