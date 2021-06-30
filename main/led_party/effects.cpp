#include "effects.hpp"
#include "partyman.hpp"

void drive_effect(led_strip_t * strips,uint step_millis, struct effect effect){
    for(int step=0;step<effect.repitions;step++){
        effect.draw(strips,step);
        vTaskDelay(step_millis / portTICK_PERIOD_MS);
    }
}

void effect_spread_pixel_draw(led_strip_t * strips, uint32_t step){
    irgb_t walking_color = 0xFF44CC;
    for(uint i=0; i<amount_strips; i++){
        if(step<=strip_lengths[i]){
            led_strip_addto_pixel_color(strips+i,step,walking_color);
            led_strip_addto_pixel_color(strips+i,step-1,invert(walking_color));
        }
    }
}


void effect_walk_pixel_draw(led_strip_t * strips, uint32_t step){
    irgb_t walking_color = 0xFF44CC;

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
    .repitions = LENGTH_MAIN_L+LENGTH_SDDL+LENGTH_DIAG_L+1,
    .draw = effect_walk_pixel_draw
};
effect effect_spread_pixel{
    .repitions = LENGTH_MAIN,
    .draw = effect_spread_pixel_draw  
};