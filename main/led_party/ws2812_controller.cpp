#include "Ws2812_controller.hpp"

const uint8_t LED_STRIP_RMT_INTR_NUMs[] = {9, 12, 13, 17, 18, 19, 20, 21, 23};
Ws2812_controller::Ws2812_controller(){return;}
Ws2812_controller::Ws2812_controller(gpio_num_t strip_data_pin, uint length, uint8_t channel)
{
    this->length = length;
    led_strip_buf =(irgb_t *) malloc(length*sizeof(irgb_t));
    sema = xSemaphoreCreateBinary();
    led_strip = {
        .led_strip_length = length,
        .gpio = strip_data_pin,
        .rmt_channel = (rmt_channel_t) channel,
        .rmt_interrupt_num = LED_STRIP_RMT_INTR_NUMs[channel],
        .led_strip_buf = led_strip_buf,
        .access_semaphore = sema
    };
    bool led_init_ok = led_strip_init(&led_strip);
    if (led_init_ok){
        printf("\nled strip initialized\n");
    }
    *temporary_pixel=0;
    return;
}

Ws2812_controller::~Ws2812_controller()
{
}

void Ws2812_controller::update(){
    //led_strip_show(&led_strip);
}
void Ws2812_controller::reset(){
    led_strip_clear(&led_strip);
}

void Ws2812_controller::set_color(uint32_t index, irgb_t iRGB){
    led_strip_set_pixel_color(&led_strip,index, iRGB);
}
irgb_t Ws2812_controller::get_color(uint32_t index){
    led_strip_get_pixel_color(&led_strip,index, temporary_pixel);
    return *temporary_pixel;
}
void Ws2812_controller::change_color(uint32_t index, irgb_t iRGB){
    iRGB += get_color(index);
    set_color(index, iRGB);
}