#include "partyman.hpp"
#include "led_strip/led_strip.h"






const uint8_t LED_STRIP_RMT_INTR_NUMs[] = {9, 12, 13, 17, 18, 19, 20, 21, 23};
led_strip_t init_new_strip(gpio_num_t strip_data_pin, uint length, uint8_t channel){
    irgb_t* led_strip_buf = (irgb_t *) malloc(length*sizeof(irgb_t)); //normales array ist nicht gut weil er dann die adresse recycled, aber cool wenn man den programmspeicer als farben zeigen will
    xSemaphoreHandle sema = xSemaphoreCreateBinary();
    led_strip_t led_strip = {
        .led_strip_length = length,
        .gpio = strip_data_pin,
        .rmt_channel = (rmt_channel_t) channel,
        .rmt_interrupt_num = LED_STRIP_RMT_INTR_NUMs[channel],
        .led_strip_buf_1 = led_strip_buf,
        .access_semaphore = sema
    };
    bool led_init_ok = led_strip_init(&led_strip);
    led_strip_clear(&led_strip);
    if (led_init_ok){
        printf("\nled strip initialized\n");
    }
    return led_strip;
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
    LENGTH_REAR_T
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
        strips[i] = init_new_strip(strip_pins[i],strip_lengths[i],i);
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
    for(uint i=0; i<amount_strips; i++){
        if(step<strip_lengths[i]){
            led_strip_set_pixel_color(strips+i,step,0xFF44CC);
        }
    }
}

effect effect_spread_pixel{
    .repitions = LENGTH_MAIN_L,
    .draw = effect_spread_pixel_draw
};

void led_party_task(void *arg){

    led_strip_t strips[amount_strips];
    initialize_strips(strips);

    uint32_t running_value = 0;
    while (1)
    {++running_value;
        drive_effect(strips,50,effect_spread_pixel);
    }
    
}