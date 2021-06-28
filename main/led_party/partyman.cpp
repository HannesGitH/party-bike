#include "partyman.hpp"

//Ws2812_controller stripe_DIAG = Ws2812_controller(DATA_PIN_DIAG,LENGTH_DIAG,1);
//Ws2812_controller stripe_FRNT = Ws2812_controller(DATA_PIN_FRNT,LENGTH_FRNT,2);
//Ws2812_controller stripe_SDDL = Ws2812_controller(DATA_PIN_SDDL,LENGTH_SDDL,3);
//Ws2812_controller stripe_REAR = Ws2812_controller(DATA_PIN_REAR,LENGTH_REAR_B,4);

Partyman::Partyman(/* args */)
{
    printf(". you got the oparty man. ");
    stripe_MAIN = Ws2812_controller(DATA_PIN_MAIN,LENGTH_MAIN,0);
}

Partyman::~Partyman()
{
}

/*
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
*/

void Partyman::walk_pixels(){
    uint32_t i = 0;
    while (1)
    {
        //stripe_MAIN.reset();
        //stripe_DIAG.reset();
        //stripe_REAR.reset();
        //stripe_FRNT.reset();
        //stripe_SDDL.reset();
        //stripe_MAIN.change_color(i,iRGB(20,90,200));
        //stripe_DIAG.change_color(i,iRGB(20,90,200));
        //stripe_REAR.change_color(i,iRGB(20,90,200));
        //stripe_FRNT.change_color(i,iRGB(20,90,200));
        //stripe_SDDL.change_color(i,iRGB(20,90,200));
        //vTaskDelay(50/portTICK_PERIOD_MS);
        //stripe_MAIN.update();
        //stripe_DIAG.update();
        //stripe_REAR.update();
        //stripe_FRNT.update();
        //stripe_SDDL.update();
        i=(i+1)%LENGTH_MAIN;
    }
}

/*
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
*/