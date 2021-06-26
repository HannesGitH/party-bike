#include "led_party_man.hpp"

#define LED_STRIP_RMT_INTR_NUM 19U

Led_party_man::Led_party_man(gpio_num_t strip_data_pin, uint length, gpio_num_t high_v_pin)
{
    sema = xSemaphoreCreateBinary();
    led_strip = {
        .rgb_led_type = RGB_LED_TYPE_WS2812,
        .led_strip_length = length,
        .gpio = strip_data_pin,

        .rmt_channel = RMT_CHANNEL_3,
        .rmt_interrupt_num = LED_STRIP_RMT_INTR_NUM,
        .showing_buf_1 = false,
        .led_strip_buf_1 = led_strip_buf_1,
        .led_strip_buf_2 = led_strip_buf_2,
        .access_semaphore = sema
    };
    printf("jetzt wirds spannend\n");
    printf("address is %d, and its length %d \n",(int) &led_strip, (&led_strip)->led_strip_length);
    bool led_init_ok = led_strip_init(&led_strip);
    if (led_init_ok){
        printf("\nled strip initialized\n");
    }
    printf("moiii\n");
    led_strip_show(&led_strip);
    printf("moiii\n");
    return;
}

Led_party_man::~Led_party_man()
{
}


void Led_party_man::set_color(uint32_t index, led_color_t RGB){
    //led_strip_set_pixel_color(&led_strip,index, &RGB);
    led_strip_set_pixel_rgb(&led_strip,3, 12,234,56);
    printf("rgbStrip updated? its address is %d \n",(int) &led_strip);
    led_strip_show(&led_strip);
}