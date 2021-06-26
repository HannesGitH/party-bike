#include "led_party_handle.hpp"

#define LED_STRIP_LENGTH 14U
#define DATA_PIN GPIO_NUM_12

#define HIGH_V_PIN GPIO_NUM_13


void walk_pixel(Led_party_man* partyman){
    while (1)
    {
        for (int i = 0; i < LED_STRIP_LENGTH; i++)
        {
            partyman->set_color(i,{
                .red = 255,
                .green = 200,
                .blue = 10
            });
            partyman->set_color((i-1)%LED_STRIP_LENGTH,{
                .red = 55,
                .green = 5,
                .blue = 1
            });
            partyman->set_color((i-2)%LED_STRIP_LENGTH,{
                .red = 55,
                .green = 5,
                .blue = 1
            });
            partyman->update();
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
}



void led_party_task(void *arg){

    Led_party_man partyman = Led_party_man(DATA_PIN,LED_STRIP_LENGTH, HIGH_V_PIN);

    walk_pixel(&partyman);
    while (1)
    {
        partyman.set_color(3,{
            .red = 255,
            .green = 200,
            .blue = 10
        });
        vTaskDelay(500 / portTICK_PERIOD_MS);
        partyman.set_color(3,{
            .red = 55,
            .green = 200,
            .blue = 100
        });
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

}

