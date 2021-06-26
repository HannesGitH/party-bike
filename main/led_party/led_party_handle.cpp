#include "led_party_handle.hpp"

#define LED_STRIP_LENGTH 14U
#define DATA_PIN GPIO_NUM_12


#define LED_STRIP_RMT_INTR_NUM 19U


static struct led_color_t led_strip_buf_1[LED_STRIP_LENGTH];
static struct led_color_t led_strip_buf_2[LED_STRIP_LENGTH];

xSemaphoreHandle sema = xSemaphoreCreateBinary();

static struct led_strip_t led_strip = {
    .rgb_led_type = RGB_LED_TYPE_WS2812,
    .led_strip_length = LED_STRIP_LENGTH,
    .gpio = DATA_PIN,

    .rmt_channel = RMT_CHANNEL_3,
    .rmt_interrupt_num = LED_STRIP_RMT_INTR_NUM,
    .showing_buf_1 = false,
    .led_strip_buf_1 = led_strip_buf_1,
    .led_strip_buf_2 = led_strip_buf_2,
    .access_semaphore = sema
};

void led_party_task(void *arg){
    bool led_init_ok = led_strip_init(&led_strip);
    if (led_init_ok){
        printf("\nled strip initialized\n");
    }
    led_strip_set_pixel_rgb(&led_strip,3,250,0,120);
    led_strip_show(&led_strip);

    while (1)
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    
}


