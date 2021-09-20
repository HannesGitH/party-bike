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

struct EffectArgMan{
    EffectWithArg * effects;
    uint8_t len;
    Partyman * pm;
    bool loop;
};

EffectArgMan efar;
QueueHandle_t effectQueue = xQueueCreate(5,sizeof(EffectWithArg));


void effectLoop(void * arg){

    bool ranOnce = false;
    EffectArgMan efar;

    for(;;){
        vTaskDelay(50/portTICK_PERIOD_MS);
        if(uxQueueMessagesWaiting(effectQueue)){
            Serial.println("received a new effect");
            xQueueReceive(effectQueue,&efar,10); //TODO: queue.c:1443 (xQueueGenericReceive)- assert failed!
            ranOnce = false;
        }
        if(efar.len == NULL)continue;      //nothing received yet
        if (efar.len == 0)continue;        //no effects
        if(ranOnce && !efar.loop)continue; //not looping and we already ran the effects

        Effect effs[efar.len];
        void *args[efar.len];

        for(int i = 0; i<efar.len ;i++){
            effs[i]=efar.effects[i].eff;
            args[i]=efar.effects[i].arg;
        }       

        efar.pm->driveEffects(effs,efar.len,args,false); 
        ranOnce = true;
    }
}


Partyman::Partyman()
{ 
    fullbuf = (irgb_t *) malloc(LENGTH_TOTAL*sizeof(irgb_t));
    mutexies = (xSemaphoreHandle *) malloc(LENGTH_TOTAL*sizeof(xSemaphoreHandle));
    initialize_strips(strips);

    speed=20;
    
    //effectQueue = xQueueCreate(5,sizeof(EffectWithArg));
    xTaskCreate(effectLoop,"effectLoop",2048,NULL,1,&loopHandle);//idk if this works or somehow weirds out because class members..

    Serial.println("partymaaaan");
    
    // drive_effect(strips,50,effect_init_rainbow);
    // drive_effect(strips,30,effect_change_hue);
    
    //void ** extra_effect_args = (void**) malloc(sizeof(void*)*10);
    struct Effect inits[] = {effect_init_rainbow, effect_walk_pixel}; 
    driveEffects(inits,2,NULL,false);
    drive_effect(strips,50,effect_set_color,&black);
    //test();
    return;
}

Partyman::~Partyman()
{
    reset();
    free(fullbuf);
}

void Partyman::loopEffects(EffectWithArg effects[],uint8_t len){
    efar = {.effects = effects,.len = len,.pm = this,.loop=true};
    xQueueSend(effectQueue,(void *)&efar,10);
    Serial.println("sent");
}

void Partyman::stopLoop(){
    runEffects(NULL,0);
}

void Partyman::runEffects(EffectWithArg effects[],uint8_t len){
    efar = {.effects = effects,.len = len,.pm = this,.loop=true};
    xQueueSend(effectQueue,(void *)&efar,10);
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
        fullbuf[i]=change_hue(iRGB(0xF0,0x0F,0x00),(float)i*7.8);
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

void Partyman::driveEffects(Effect * effects, uint8_t amount, void * extra_args_p[], bool run_all_in_every_step){
    drive_effects(strips,speed,effects,amount,extra_args_p,run_all_in_every_step);
    Serial.println("drove em");return;
}

void Partyman::reset(){
    stopLoop();
    drive_effect(strips,50,effect_set_color,&black);
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