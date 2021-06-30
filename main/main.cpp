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
#include "led_party/led_party_handle.hpp"


TaskHandle_t led_party_task_handle;

extern void led_party_task(void *arg);

void init_stuff(){
    printf("initializing..\n");

    if( 
        xTaskCreate(
            led_party_task,         //task to run
            "led_party_task",       //name of task
            4096,                   //stack size
            NULL,                   //arg pointer
            configMAX_PRIORITIES-5, //interrupt prio
            &led_party_task_handle  //handle (to kill task)
        )
    ){
        printf("PARTY thread started\n");
    }else{
        printf("party thread couldnt be started :(\n");
    }
    
}


#define BLINK_GPIO GPIO_NUM_2 //onboard_led
extern "C"{

//entry
void app_main(void)
{
    init_stuff();

    /* //blink led to show its running
        gpio_reset_pin(BLINK_GPIO);
        gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

        while(1) {
            gpio_set_level(BLINK_GPIO, 0);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            gpio_set_level(BLINK_GPIO, 1);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        } */
}
}