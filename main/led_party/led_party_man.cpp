#include "led_party_man.hpp"

#define LED_STRIP_RMT_INTR_NUM 19U

Led_party_man::Led_party_man(gpio_num_t strip_data_pin, uint length, gpio_num_t high_v_pin)
{
    led_strip_buf_1 =(irgb_t *) malloc(length*sizeof(irgb_t));
    sema = xSemaphoreCreateBinary();
    led_strip = {
        .led_strip_length = length,
        .gpio = strip_data_pin,
        .rmt_channel = RMT_CHANNEL_3,
        .rmt_interrupt_num = LED_STRIP_RMT_INTR_NUM,
        .led_strip_buf_1 = led_strip_buf_1,
        .access_semaphore = sema
    };
    bool led_init_ok = led_strip_init(&led_strip);
    if (led_init_ok){
        printf("\nled strip initialized\n");
    }
    *temporary_pixel=0;
    return;
}

Led_party_man::~Led_party_man()
{
}

void Led_party_man::update(){
    //led_strip_show(&led_strip);
}
void Led_party_man::reset(){
    led_strip_clear(&led_strip);
}

void Led_party_man::set_color(uint32_t index, irgb_t iRGB){
    led_strip_set_pixel_color(&led_strip,index, iRGB);
}
irgb_t Led_party_man::get_color(uint32_t index){
    led_strip_get_pixel_color(&led_strip,index, temporary_pixel);
    return *temporary_pixel;
}
void Led_party_man::change_color(uint32_t index, irgb_t iRGB){
    iRGB += get_color(index);
    set_color(index, iRGB);
}