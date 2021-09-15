#include "partyman.hpp"
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
    mutexies = (xSemaphoreHandle *) malloc(LENGTH_TOTAL*sizeof(xSemaphoreHandle));
    initialize_strips(strips);
    Serial.println("partymaaaan");
    
    //drive_effect(strips,50,effect_walk_pixel);
    //test();
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
    const uint8_t factor = 2;
    for (uint16_t i = 0; i < LENGTH_TOTAL; i++)
    {
        fullbuf[i]=iRGB(0x90*i,0x03*i,134);
    }
    sendBuffer(fullbuf);
    for (uint16_t i = 0; i < LENGTH_TOTAL; i++)
    {
        fullbuf[i]={.r=0x00, .g=0x00, .b=0x00, .i=0x00};
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    sendBuffer(fullbuf);
    //drive_effect(strips,50,effect_init_rainbow);
}


const uint8_t LED_STRIP_RMT_INTR_NUMs[] = {9, 12, 13, 17, 18, 19, 20, 21, 23};
bool Partyman::init_new_strip(led_strip_t * led_strip_p, gpio_num_t strip_data_pin, uint length, uint8_t channel){
    irgb_t* led_strip_buf = (irgb_t *) malloc(length*sizeof(irgb_t)); //normales array ist nicht gut weil er dann die adresse recycled, aber cool wenn man den programmspeicer als farben zeigen will
    *led_strip_p = {
        .led_strip_length = length,
        .gpio = strip_data_pin,
        .rmt_channel = (rmt_channel_t) channel,
        .rmt_interrupt_num = LED_STRIP_RMT_INTR_NUMs[channel],
        .led_strip_buf = led_strip_buf,
    };
    led_strip_p->access_semaphore=mutexies[channel];
    bool led_init_ok = led_strip_init(led_strip_p);
    led_strip_clear(led_strip_p);
    if (led_init_ok){
        printf("led strip initialized\n");
        return true;
    }
    return false;
}


void Partyman::initialize_strips(led_strip_t * strips){
    for(int i=0; i<amount_strips; i++){
        mutexies[i] = xSemaphoreCreateBinary();
        init_new_strip(strips+i,strip_pins[i],strip_lengths[i],i);
        vTaskDelay(1);
    }
}
void Partyman::led_strips_clear(led_strip_t * strips){
    for(int i=0; i<amount_strips; i++){
        led_strip_clear(strips+i);
    }
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