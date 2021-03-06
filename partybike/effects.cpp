#include "effects.hpp"
//#include <stddef.h>
#include "stripdata.hpp"
#include "math.hpp"
#include "math.h"
#include <Arduino.h>
//#include "freertos/FreeRTOS.h"

void drive_effect(led_strip_t * strips,uint step_millis, Effect effect, void * extra_args_p){
    for(int step=0;step<effect.repetitions;step++){
        effect.draw(strips,step,extra_args_p);
        vTaskDelay(step_millis / portTICK_PERIOD_MS);
    }
}

void drive_effects(led_strip_t * strips,uint step_millis, Effect * effects, uint8_t amount, void * extra_args_p[], bool run_all_in_every_step){
    uint32_t max_reps = 0;
    ////Serial.printf("running %d effects \n",amount);
    for (uint16_t i = 0; i < amount; i++)
    {
        max_reps = max(max_reps,effects[i].repetitions);
        //// Serial.println(effects[i].repetitions);
       //// Serial.println(max_reps);
    }
    
    for(int step=0;step<max_reps;step++){
        for (uint16_t i = 0; i < amount; i++)
        {
            ////Serial.printf("step %d, effect %d \n",step, i);
            if(run_all_in_every_step || effects[i].repetitions > step)
            effects[i].draw(strips,step, extra_args_p ? extra_args_p[i] : NULL);
        }
        vTaskDelay(step_millis / portTICK_PERIOD_MS);
    }
}

// //TODO why would i want this?
// void effect_spread_pixel_draw(led_strip_t * strips, uint32_t step, void* extra_args_p){
//     irgb_t walking_color = iRGB(0xFF,0x44,0xCC);
//     for(uint i=0; i<amount_strips; i++){
//         if(step<=strip_lengths[i]){
//             led_strip_aset_pixel_color(strips+i,step,walking_color);
//             led_strip_aset_pixel_color(strips+i,step-1,invert(walking_color));
//         }
//     }
// }

void draw_iterated_pixel(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = extra_args_p ? *(irgb_t*) extra_args_p : irgb_t{.r = 0xFF, .g = 0x44, .b = 0xCC, .i = 0xF0};
    bool isLeft = walking_color.i & 0x80 ;
    bool isRight = walking_color.i & 0x40 ;
    bool isCenter = walking_color.i & 0x20 ;
    bool add = walking_color.i & 0x10 ;
    if(step==0)Serial.printf("%x: %x,%x,%x,%x\n",walking_color.i,isLeft,isRight,isCenter,add);

    if(step<LENGTH_MAIN_L){
        //main_r
        if(isRight)     led_strip_aset_pixel_color(strips+MAIN,step,walking_color,add);
        //main_l
        if(isLeft)      led_strip_aset_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T-step-1,walking_color,add);
        //rear_t
        if(isCenter)    led_strip_aset_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T+step,walking_color,add);
    }

    if (LENGTH_MAIN_L<step&&step<(LENGTH_MAIN_L+LENGTH_DIAG_L+1))
    {
        
        uint32_t step2=step-LENGTH_MAIN_L;
        ////Serial.printf("-> (%d->%d)\t {r: %d g:%d b: %d}\n", step,LENGTH_DIAG_R-step2, walking_color.r, walking_color.g, walking_color.b);
        //diag_r
        if(isRight)     led_strip_aset_pixel_color(strips+DIAG,LENGTH_DIAG_R-step2,walking_color,add);
        //diag_l
        if(isLeft)      led_strip_aset_pixel_color(strips+DIAG,LENGTH_DIAG_R+step2-1,walking_color,add);
        //frnt
        if(isCenter)    led_strip_aset_pixel_color(strips+FRNT,step2-1,walking_color,add);
    }  
    
    if (LENGTH_MAIN_L+LENGTH_DIAG_L<step&&step<LENGTH_MAIN_L+LENGTH_DIAG_L+LENGTH_SDDL)
    {
        uint32_t step3=step-(LENGTH_MAIN_L+LENGTH_DIAG_L);
        //sddl
        if(isCenter)    led_strip_aset_pixel_color(strips+SDDL,LENGTH_SDDL-step3,walking_color,add);
        //rear_b
        if(isCenter)    led_strip_aset_pixel_color(strips+REAR,LENGTH_REAR_B-step3,walking_color,add);
    }
}

void set_iterated_pixel(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = extra_args_p ? *(irgb_t*) extra_args_p : irgb_t{.r = 0xFF, .g = 0x44, .b = 0xCC, .i = 0xF0};
    ((irgb_t*)&walking_color)->i &= 0xEF; //add-bit auf 0 setzen
    return draw_iterated_pixel(strips,step,&walking_color);
}


void remove_iterated_pixel(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = extra_args_p ? *(irgb_t*) extra_args_p : irgb_t{.r = 0xFF, .g = 0x44, .b = 0xCC, .i = 0xF0};
    bool isLeft = walking_color.i & 0x80 ;
    bool isRight = walking_color.i & 0x40 ;
    bool isCenter = walking_color.i & 0x20 ;
    bool add = true;// oder !(walking_color.i & 0x10) ; // da wir wenn wir gesettet haben auch um den wert wieder zur??ck??ndern wollen und nicht aufs negative setten eigtl immer true
    irgb_t inverted = invert(walking_color);
    ////Serial.printf("{r: %d g:%d b: %d}\t->\t{r: %d g:%d b: %d}\n", walking_color.r, walking_color.g, walking_color.b, inverted.r, inverted.g, inverted.b);

    if(0<step&&step<LENGTH_MAIN_L){
        //undo
        //main_r
        if(isRight)     led_strip_aset_pixel_color(strips+MAIN,step-1,inverted,add);
        //main_l
        if(isLeft)      led_strip_aset_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T-step,inverted,add);
        //rear_t
        if(isCenter)    led_strip_aset_pixel_color(strips+MAIN,strip_lengths[MAIN]-LENGTH_REAR_T+step-1,inverted,add);    
    }
 
    if(LENGTH_MAIN_L<(step)&&(step)<(LENGTH_MAIN_L+LENGTH_DIAG_L+1)){//undo
        uint32_t step2=step-LENGTH_MAIN_L;
        //diag_r
        if(isRight)     led_strip_aset_pixel_color(strips+DIAG,LENGTH_DIAG_L-step2,inverted,add);
        //diag_l
        if(isLeft)      led_strip_aset_pixel_color(strips+DIAG,LENGTH_DIAG_R+step2-1,inverted,add);
        //frnt
        if(isCenter)    led_strip_aset_pixel_color(strips+FRNT,step2+3,inverted,add);
    }
    
    if (LENGTH_MAIN_L+LENGTH_DIAG_L<(step)&&(step)<LENGTH_MAIN_L+LENGTH_DIAG_L+LENGTH_SDDL){//undo
        uint32_t step3=step-(LENGTH_MAIN_L+LENGTH_DIAG_L);
        //sddl
        if(isCenter)    led_strip_aset_pixel_color(strips+SDDL,LENGTH_SDDL-step3,inverted,add);
        //rear_b
        if(isCenter)    led_strip_aset_pixel_color(strips+REAR,LENGTH_REAR_B-step3,inverted,add);
    }
}

void effect_walk_pixel_draw(led_strip_t * strips, uint32_t step, void* extra_args_p){
    irgb_t walking_color = extra_args_p ? *(irgb_t*) extra_args_p : irgb_t{.r = 0xFF, .g = 0x44, .b = 0xCC, .i = 0xF0};
    
    const uint16_t max = LENGTH_MAIN_L+LENGTH_SDDL+LENGTH_DIAG_L+1;
    if(step!=max)draw_iterated_pixel(strips, step, extra_args_p);
    if(step!=0)remove_iterated_pixel(strips, step-1, extra_args_p);
}

Effect effect_walk_pixel{
    .repetitions = LENGTH_MAIN_L+LENGTH_SDDL+LENGTH_DIAG_L+1,
    .draw = effect_walk_pixel_draw
};
// Effect effect_spread_pixel{
//     .repetitions = LENGTH_MAIN,
//     .draw = effect_spread_pixel_draw  
// };

void effect_walking_colorline_draw(led_strip_t * strips, uint32_t step, void* extra_args_p){
    uint8_t length = extra_args_p ? *(uint8_t*) extra_args_p : 10;
    irgb_t color = (irgb_t)iRGB(0xFF,0x44,0xCC);
    for (uint8_t i = 0; i < length; i++)
    {
        float sat = (float)(length-i)/(float)length;
        irgb_t current_color = change_hsv(color,(float)i*10,sat,sat);
        effect_walk_pixel.draw(strips,step-i,&current_color);
    }
}

Effect effect_walking_colorline{
    .repetitions = LENGTH_MAIN_L+LENGTH_SDDL+LENGTH_DIAG_L+1,
    .draw = effect_walking_colorline_draw
};

Effect effect_set_color{
    .repetitions = LENGTH_MAIN_L+LENGTH_SDDL+LENGTH_DIAG_L,
    .draw = set_iterated_pixel
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
        led_strip_set_pixel_color(strips+REAR,i,(irgb_t)iRGB(0xFF,0x00,0x00));
    }
    //turn rear_t red as well
    for (uint8_t i = 0; i < LENGTH_REAR_T; i++)
    {
        led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN-LENGTH_REAR_T+i,(irgb_t)iRGB(0xFF,0x00,0x00));
    }

    //turn front white
    for (uint8_t i = 0; i < strip_lengths[FRNT]-3; i++)//there r 3 pixels not facing the front on the frnt strip
    {
        led_strip_set_pixel_color(strips+FRNT,i,(irgb_t)iRGB(0xFF,0xFF,0xFF));
    }
    /* 
    //saddle is also facing the front so turn it white as well
    for (uint8_t i = 0; i < strip_lengths[SDDL]; i++)
    {
        led_strip_set_pixel_color(strips+SDDL,i,0xFFFFFF);
    }
     */
    //turn other pixels facing the front white
    led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN_R  ,(irgb_t)iRGB(0xFF,0xFF,0xFF));
    led_strip_set_pixel_color(strips+MAIN,LENGTH_MAIN_R-1,(irgb_t)iRGB(0xFF,0xFF,0xFF));
    led_strip_set_pixel_color(strips+DIAG,LENGTH_DIAG_R  ,(irgb_t)iRGB(0xFF,0xFF,0xFF));
    led_strip_set_pixel_color(strips+DIAG,LENGTH_DIAG_R-1,(irgb_t)iRGB(0xFF,0xFF,0xFF));
    
    //turn other pixels facing the back red
    led_strip_set_pixel_color(strips+FRNT,LENGTH_FRNT-1,(irgb_t)iRGB(0xFF,0x00,0x00));
    led_strip_set_pixel_color(strips+FRNT,LENGTH_FRNT-2,(irgb_t)iRGB(0xFF,0x00,0x00));
    led_strip_set_pixel_color(strips+FRNT,LENGTH_FRNT-3,(irgb_t)iRGB(0xFF,0x00,0x00));
}

Effect effect_streetlight{
    .repetitions = 1,
    .draw = effect_streetlight_draw
};


void effect_init_rainbow_draw(led_strip_t * strips, uint32_t step, void* total_hue_rotations_p){
    Serial.println("rainbowing");
    int total_hue_rotations = 2;//total_hue_rotations_p ? *(int*) total_hue_rotations_p : 1;
    uint8_t pixels = LENGTH_MAIN_L+LENGTH_DIAG_L+LENGTH_SDDL;
    float hue_change = fmod(360*(((float)total_hue_rotations)/pixels),360);
    irgb_t color = (irgb_t)iRGB(0x00,0x00,0xFE);
    for (uint8_t i = 0; i < pixels; i++)
    {
        irgb_t current_color = change_hue(color,i*hue_change);
        //Serial.printf("turning pixel \t %d \t into {r: %d g:%d b: %d}\n", i, current_color.r, current_color.g, current_color.b);
        draw_iterated_pixel(strips,i,&current_color);
    }
}

Effect effect_init_rainbow{
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

Effect effect_change_hue{
    .repetitions = 360,
    .draw = effect_change_hue_draw
};


void effect_blink_draw(led_strip_t * strips, uint32_t step, void* right){
    uint8_t flags = 0x80;// (0x80 >> *(bool*)right);
    if(right && *(bool*)right){flags = 0x40;}

    if(step<effect_walk_pixel.repetitions){
        irgb_t color = {.r = 0xFF, .g = 0xCD, .b = 0x03, .i=flags};
        for (uint16_t i = 0; i < step; i++)
        {
            set_iterated_pixel(strips,i,&color);
        }
    }else{
        irgb_t color = black;
        color.i=flags;
        for (uint16_t i = 0; i < effect_walk_pixel.repetitions; i++)
        {
            set_iterated_pixel(strips,i,&color);
        }
    } 
}

Effect effect_blink{
    .repetitions = effect_walk_pixel.repetitions*2,
    .draw = effect_blink_draw
};
