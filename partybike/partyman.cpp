#include "partyman.hpp"
#include "led_party_handle.hpp"
#include "effects.hpp"
#include "math.hpp"

Partyman::Partyman()
{
    initialize_strips(strips);
}

Partyman::~Partyman()
{
}

void Partyman::runEffects(effectWithArg effects[]){
    int arrLength = sizeof(effects)/sizeof(effects[0]);
    effect effs[arrLength];
    void * args[arrLength];
    for(int i = 0; i<sizeof(effects)/sizeof(effects[0]) ;i++){
        effs[i]=*(effects[i].eff);
        args[0]=effects[0].arg;
    }
    drive_effects(strips,20,effs,arrLength,args,false); 
}

void Partyman::sendBuffer(irgb_t buffer[LENGTH_TOTAL]){
    for(int step = 0; step < *(std::max_element(strip_lengths,strip_lengths+amount_strips)); step++){
        for(uint i=0; i<amount_strips; i++){
            if(step<=strip_lengths[i]){
                led_strip_set_pixel_color(strips+i,step,buffer[step+bufferOffset(i)]);
            }
        }
    }
}

int bufferOffset(uint16_t stripnum){
    uint16_t acc = 0;
    for (uint8_t i = 0; i < stripnum; i++)
    {
        acc+=strip_lengths[i];
    }
    return acc;
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