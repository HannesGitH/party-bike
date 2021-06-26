#include "led_party_handle.hpp"

#define LED_STRIP_LENGTH 14U
#define DATA_PIN GPIO_NUM_12

#define HIGH_V_PIN GPIO_NUM_13


void walk_pixel(Led_party_man* partyman){
    for (int i = 0; i < LED_STRIP_LENGTH; i++)
        {
            irgb_t rgb = iRGB(255,80,i*5);
            partyman->set_color(i,rgb);
        }
    while (1)
    {
        for (int i = 0; i < LED_STRIP_LENGTH; i++)
        {
            partyman->change_color(i,iRGB(1,2,3));
        }
        vTaskDelay(50/portTICK_PERIOD_MS);
        partyman->update();
    }
}



void led_party_task(void *arg){

    Led_party_man partyman = Led_party_man(DATA_PIN,LED_STRIP_LENGTH, HIGH_V_PIN);

    walk_pixel(&partyman);
}

