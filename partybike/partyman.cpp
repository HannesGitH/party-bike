#include "partyman.hpp"
#include "led_party_handle.hpp"
#include "effects.hpp"
#include "math.hpp"
#include "led_strip.h"
#include <algorithm>
#include <Arduino.h>

int bufferOffset(uint16_t stripnum){
    uint16_t acc = 1;
    for (uint8_t i = 0; i < stripnum; i++)
    {
        acc+=strip_lengths[i];
    }
    return acc-1;
}

Partyman::Partyman()
{ 
    fullbuf = (irgb_t *) malloc(LENGTH_TOTAL*sizeof(irgb_t));
    initialize_strips(strips);
    Serial.println("partymaaaan");
    
    //drive_effect(strips,50,effect_walk_pixel);
    test();
    return;
}

Partyman::~Partyman()
{
    free(fullbuf);
}

void Partyman::runEffects(EffectWithArg effects[],uint8_t len){
    int arrLength = len;//sizeof(effects)/sizeof(effects[0]);
    // Serial.println(effects[0].eff->repetitions);
    Effect effs[arrLength];
    void * args[arrLength];
    for(int i = 0; i<arrLength ;i++){
        effs[i]=*(effects[i].eff);
        // Serial.println(effects[i].eff->repetitions);
        // Serial.println((*(effects[i].eff)).repetitions);
        // Serial.println(effs[i].repetitions);
        args[0]=effects[0].arg;
    }
    // Serial.println(arrLength);
    drive_effects(strips,20,&effs[0],arrLength,args,false); 
    Serial.println("drove em");return;
}

void Partyman::sendBuffer(irgb_t buffer[LENGTH_TOTAL]){
    uint16_t longest_length = *(std::max_element(strip_lengths,strip_lengths+amount_strips));
    for(int step = 0; step < longest_length; step++){
        for(uint i=0; i<amount_strips; i++){
            if(step<=strip_lengths[i]){
                led_strip_set_pixel_color(strips+i,step,buffer[step+bufferOffset(i)]);
            }
        }
    }
}

void Partyman::test(){
    for (uint16_t i = 0; i < LENGTH_TOTAL; i++)
    {
        fullbuf[i]={.r=0xFF, .g=0xFF, .b=0xFF, .i=0x00};
    }
    sendBuffer(fullbuf);
    for (uint16_t i = 0; i < LENGTH_TOTAL; i++)
    {
        fullbuf[i]={.r=0x00, .g=0x00, .b=0x00, .i=0x00};
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    sendBuffer(fullbuf);
    //drive_effect(strips,50,effect_init_rainbow,NULL);
}


// ich brauch mal hilfe mit c/c++:

// ich habe ein array voller 
// ```c
// struct foo{
//     /*stuff*/
//     Bar * bar; 
// }
// ```
// also 
// ```c
// foo arrOfFoos[] = /**/;
// ``` 
// und möchte nun ein array mit den entsprechenden `Bar`s übergeben, geht das ohne zusätzlichen speicheraufwand im sinne von 
// ```c
// Bar * arrOfBars = arrOfFoos->bar;
// ```