#include "partyman.hpp"
#include "led_party_handle.hpp"
#include "led_strip/led_strip.h"

const xSemaphoreHandle mutexies[] = {
    xSemaphoreCreateBinary(),
    xSemaphoreCreateBinary(),
    xSemaphoreCreateBinary(),
    xSemaphoreCreateBinary(),
    xSemaphoreCreateBinary()
};

const uint8_t LED_STRIP_RMT_INTR_NUMs[] = {9, 12, 13, 17, 18, 19, 20, 21, 23};
bool init_new_strip(led_strip_t * led_strip_p, gpio_num_t strip_data_pin, uint length, uint8_t channel){
    irgb_t* led_strip_buf = (irgb_t *) malloc(length*sizeof(irgb_t)); //normales array ist nicht gut weil er dann die adresse recycled, aber cool wenn man den programmspeicer als farben zeigen will
    *led_strip_p = {
        .led_strip_length = length,
        .gpio = strip_data_pin,
        .rmt_channel = (rmt_channel_t) channel,
        .rmt_interrupt_num = LED_STRIP_RMT_INTR_NUMs[channel],
        .led_strip_buf = led_strip_buf,
        .access_semaphore = mutexies[channel]
    };
    bool led_init_ok = led_strip_init(led_strip_p);
    led_strip_clear(led_strip_p);
    if (led_init_ok){
        printf("led strip initialized\n");
        return true;
    }
    return false;
}

#define amount_strips 5
enum strip_enum{
    MAIN=0,
    DIAG=1,
    FRNT=2,
    SDDL=3,
    REAR=4
};
const uint16_t strip_lengths[] = {
    LENGTH_MAIN,
    LENGTH_DIAG,
    LENGTH_FRNT,
    LENGTH_SDDL,
    LENGTH_REAR_B
};
const gpio_num_t strip_pins[] = {
    DATA_PIN_MAIN,
    DATA_PIN_DIAG,
    DATA_PIN_FRNT,
    DATA_PIN_SDDL,
    DATA_PIN_REAR
};

void initialize_strips(led_strip_t * strips){
    for(int i=0; i<amount_strips; i++){
        init_new_strip(strips+i,strip_pins[i],strip_lengths[i],i);
    }
}
void led_strips_clear(led_strip_t * strips){
    for(int i=0; i<amount_strips; i++){
        led_strip_clear(strips+i);
    }
}

struct effect{
    uint32_t repitions;
    void (*draw)(led_strip_t * strips, uint32_t step);
};

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

effect effect_spread_pixel{
    .repitions = LENGTH_MAIN,
    .draw = effect_spread_pixel_draw  
};

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
        printf("%d",step3);
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


void led_party_task(void *arg){

//apperently haben alle dieselbe adresse?
    //led_strip_t * strips = (led_strip_t *) malloc(amount_strips*sizeof(led_strip_t));
    led_strip_t strips[amount_strips];

    initialize_strips(strips);

    
    for(uint32_t running_value = 0;1;++running_value)
    {
        drive_effect(strips,50,effect_walk_pixel);
        led_strips_clear(strips);
        //led_strip_set_pixel_color(strips+DIAG,running_value%LENGTH_DIAG,(running_value%3)*0xFF3456);
        //led_strip_set_pixel_color(strips+MAIN,running_value%strip_lengths[MAIN],0x10743C);
        //led_strip_set_pixel_color(strips+MAIN,(running_value-1)%strip_lengths[MAIN],0xFF44CC);
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}
