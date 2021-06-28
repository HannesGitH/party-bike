#include "Ws2812_controller.hpp"

#define LED_STRIP_LENGTH 160U
#define DATA_PIN GPIO_NUM_12 //diagonal
#define DATA_PIN GPIO_NUM_13 //main60 //draws way to much power so esp overheats and crashes usb connection
#define DATA_PIN GPIO_NUM_14 //front
#define DATA_PIN GPIO_NUM_27 //sattelstange
#define DATA_PIN GPIO_NUM_23 //rear   //leider is det kabel schrotti, deshalb n anderer pin, wär eigtl 26 (todo)



void chilling(Ws2812_controller* partyman){
    for (int i = 0; i < LED_STRIP_LENGTH; i++)
        {
            irgb_t rgb = iRGB(i*2,i*(-1),i*5);
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


void walk_pixel(Ws2812_controller* partyman){
    uint32_t i = 0;
    while (1)
    {
        partyman->reset();
        partyman->change_color(i,iRGB(20,90,200));
        vTaskDelay(50/portTICK_PERIOD_MS);
        partyman->update();
        i=(i+1)%LED_STRIP_LENGTH;
    }
}

void test(Ws2812_controller* partyman){
    partyman->reset();
    uint32_t j = 0;
    while (1)
    {
        for (int i = 0; i < LED_STRIP_LENGTH; i++)
        {
            partyman->set_color(i,iRGB(((j%3==0)*255),((j%3==1)*255),((j%3==2)*255)));
        }
        vTaskDelay(500/portTICK_PERIOD_MS);
        partyman->update();
        j++;
    }
}

void led_party_task(void *arg){

    Ws2812_controller partyman = Ws2812_controller(DATA_PIN,LED_STRIP_LENGTH);
    test(&partyman);
}

