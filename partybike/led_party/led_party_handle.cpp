#include "stripdata.hpp"
#include "effects.hpp"
#include "led_party_handle.hpp"

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




//MARK currently depricated
void led_party_task(void *arg){

    //apperently haben alle dieselbe adresse?
    //led_strip_t * strips = (led_strip_t *) malloc(amount_strips*sizeof(led_strip_t));
    led_strip_t strips[amount_strips];

    initialize_strips(strips);

    irgb_t the_bestest_color = {.g=255,.b=255};
    uint8_t length_for_rgb_stripe = 0;
    

    void ** extra_effect_args = (void**) malloc(sizeof(void*)*10);

    extra_effect_args[1]=&length_for_rgb_stripe;

    drive_effect(strips,50,effects[2]);
    irgb_t addCol = {.r=0x12,.g=0x34,.b=0x56};
    for(uint32_t running_value = 0;1;++running_value)
    {
        the_bestest_color+=addCol;
        //drive_effect(strips,50,effect_walk_pixel,&the_bestest_color);
        //drive_effect(strips,50,effect_walking_colorline,&running_value);
        length_for_rgb_stripe = running_value;
        drive_effects(strips,50,effects+3,2,extra_effect_args+3,false); 
        //drive_effect(strips,50,effects[2]);
        //drive_effect(strips,50,effect_walking_colorline,effect_streetlight);
        //drive_effect(strips,30,effects[3]);
        led_strips_clear(strips);
        //led_strip_set_pixel_color(strips+DIAG,running_value%LENGTH_DIAG,(running_value%3)*0xFF3456);
        //led_strip_set_pixel_color(strips+MAIN,running_value%strip_lengths[MAIN],0x10743C);
        //led_strip_set_pixel_color(strips+MAIN,(running_value-1)%strip_lengths[MAIN],0xFF44CC);
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
