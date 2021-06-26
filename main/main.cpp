/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "led_strip.h"


/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO GPIO_NUM_13

#define DATA_PIN GPIO_NUM_12


#define LED_STRIP_LENGTH 14U
#define LED_STRIP_RMT_INTR_NUM 19U


static struct led_color_t led_strip_buf_1[LED_STRIP_LENGTH];
static struct led_color_t led_strip_buf_2[LED_STRIP_LENGTH];

xSemaphoreHandle sema = xSemaphoreCreateBinary();

void init_stuff(){
    printf("initializing.");
    static struct led_strip_t led_strip = {
        .rgb_led_type = RGB_LED_TYPE_WS2812,
        .led_strip_length = LED_STRIP_LENGTH,
        .gpio = DATA_PIN,

        .rmt_channel = RMT_CHANNEL_3,
        .rmt_interrupt_num = LED_STRIP_RMT_INTR_NUM,
        .showing_buf_1 = true,
        .led_strip_buf_1 = led_strip_buf_1,
        .led_strip_buf_2 = led_strip_buf_2,
        .access_semaphore = sema
    };
    printf(".");
    bool led_init_ok = led_strip_init(&led_strip);
    if (led_init_ok){
        printf("\nled strip initialized\n");
    }
    printf("setting pixel\n");
    led_strip_set_pixel_rgb(&led_strip,3,250,0,120);
    printf("showing pixel\n");
    led_strip_show(&led_strip);
    printf("%d\n",(&led_strip)->rmt_channel);
    printf("init done\n");
}

extern "C"{
void app_main(void)
{
    init_stuff();

    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, 0);
        //Delayer::get().delay_50ns(20000000);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
}