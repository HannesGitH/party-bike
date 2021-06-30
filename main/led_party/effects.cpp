#include "effects.hpp"
#include "partyman.hpp"

void drive_effect(led_strip_t * strips,uint step_millis, struct effect effect, void * extra_args_p){
    for(int step=0;step<effect.repetitions;step++){
        effect.draw(strips,step,extra_args_p);
        vTaskDelay(step_millis / portTICK_PERIOD_MS);
    }
}

void effect_spread_pixel_draw(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = 0xFF44CC;
    for(uint i=0; i<amount_strips; i++){
        if(step<=strip_lengths[i]){
            led_strip_addto_pixel_color(strips+i,step,walking_color);
            led_strip_addto_pixel_color(strips+i,step-1,invert(walking_color));
        }
    }
}


void effect_walk_pixel_draw(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = extra_args_p ? *(irgb_t*) extra_args_p : 0xFF44CC;

    if(step<LENGTH_MAIN_L){
        //main_r
        led_strip_addto_pixel_color(strips+MAIN,step,walking_color);
        //main_l
        led_strip_addto_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T-step-1,walking_color);
        //rear_t
        led_strip_addto_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T+step,walking_color);
    }
    if(0<step&&step<LENGTH_MAIN_L+1){
        //undo
        //main_r
        led_strip_addto_pixel_color(strips+MAIN,step-1,invert(walking_color));
        //main_l
        led_strip_addto_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T-step,invert(walking_color));
        //rear_t
        led_strip_addto_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T+step-1,invert(walking_color));    
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
    if(LENGTH_MAIN_L<(step-1)&&(step-1)<(LENGTH_MAIN_L+LENGTH_DIAG_L+1)){//undo
        uint32_t step2=step-LENGTH_MAIN_L-1;
        //diag_r
        led_strip_addto_pixel_color(strips+DIAG,LENGTH_DIAG_L-step2,invert(walking_color));
        //diag_l
        led_strip_addto_pixel_color(strips+DIAG,LENGTH_DIAG_R+step2-1,invert(walking_color));
        //frnt
        led_strip_addto_pixel_color(strips+FRNT,step2+3,invert(walking_color));
    }
    
    if (LENGTH_MAIN_L+LENGTH_DIAG_L<step&&step<LENGTH_MAIN_L+LENGTH_DIAG_L+LENGTH_SDDL)
    {
        uint32_t step3=step-(LENGTH_MAIN_L+LENGTH_DIAG_L);
        //sddl
        led_strip_addto_pixel_color(strips+SDDL,LENGTH_SDDL-step3,walking_color);
        //rear_b
        led_strip_addto_pixel_color(strips+REAR,LENGTH_REAR_B-step3,walking_color);
    }
    if (LENGTH_MAIN_L+LENGTH_DIAG_L<(step-1)&&(step-1)<LENGTH_MAIN_L+LENGTH_DIAG_L+LENGTH_SDDL){//undo
        uint32_t step3=step-(LENGTH_MAIN_L+LENGTH_DIAG_L)-1;
        //sddl
        led_strip_addto_pixel_color(strips+SDDL,LENGTH_SDDL-step3,invert(walking_color));
        //rear_b
        led_strip_addto_pixel_color(strips+REAR,LENGTH_REAR_B-step3,invert(walking_color));
    }
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
    irgb_t color = 0xFF;
    for (uint8_t i = 0; i < length; i++)
    {
        irgb_t current_color = change_hue(color,(float)i*10);
        effect_walk_pixel.draw(strips,step-i,&current_color);
    }
}

effect effect_walking_colorline{
    .repetitions = LENGTH_MAIN_L+LENGTH_SDDL+LENGTH_DIAG_L+1,
    .draw = effect_walking_colorline_draw
};


void effect_streetlight_draw(led_strip_t * strips, uint32_t step, void* bool__turn_off_everything_else_so_its_legal){
    bool legal = bool__turn_off_everything_else_so_its_legal ? *(bool*) bool__turn_off_everything_else_so_its_legal : false; //yo we just wanna turn the other running effects off if we reeeally wanna be legal
    
    if(legal){
        for(int i=0; i<amount_strips; i++){
            led_strip_clear(strips+i);
        }
    }

    //turn rear_b red
    for (uint8_t i = 0; i < strip_lengths[REAR]; i++)
    {
        led_strip_set_pixel_color(strips+REAR,i,0xFF0000);
    }
    //turn rear_t red as well
    for (uint8_t i = 0; i < LENGTH_REAR_T; i++)
    {
        led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN-LENGTH_REAR_T+i,0xFF0000);
    }

    //turn front white
    for (uint8_t i = 0; i < strip_lengths[FRNT]-3; i++)//there r 3 pixels not facing the front on the frnt strip
    {
        led_strip_set_pixel_color(strips+FRNT,i,0xFFFFFF);
    }
    /* 
    //saddle is also facing the front so turn it white as well
    for (uint8_t i = 0; i < strip_lengths[SDDL]; i++)
    {
        led_strip_set_pixel_color(strips+SDDL,i,0xFFFFFF);
    }
     */
    //turn other pixels facing the front white
    led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN_R  ,0xFFFFFF);
    led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN_R-1,0xFFFFFF);
    led_strip_set_pixel_color(strips+DIAG,LENGTH_DIAG_R  ,0xFFFFFF);
    led_strip_set_pixel_color(strips+DIAG,LENGTH_DIAG_R-1,0xFFFFFF);
    
    //turn other pixels facing the back red
    led_strip_set_pixel_color(strips+FRNT,LENGTH_FRNT-1,0xFF0000);
    led_strip_set_pixel_color(strips+FRNT,LENGTH_FRNT-2,0xFF0000);
}

effect effect_streetlight{
    .repetitions = 1,
    .draw = effect_streetlight_draw
};