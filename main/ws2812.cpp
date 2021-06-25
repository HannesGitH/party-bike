#include "ws2812.hpp"






/*
void test_strip(){
    for(int i=0;i<1000;i++) {
        printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, 0);
        delay_cycles(1000);
        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        delay_cycles(1000);
    }
}
*/

Ws2812::Ws2812(gpio_num_t data_pin)
{

}
Ws2812::~Ws2812()
{
    //delete &d;
}





Delayer::Delayer(){
    /* Select and initialize basic parameters of the timer */
    timer_config_t config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = 2,
    }; // default clock source is APB
    timer_init(TGROUP,TTIMER, &config);
    timer_set_counter_value(TGROUP,TTIMER, 0);
    timer_start(TGROUP,TTIMER);
    printf("timer initialized");
}

Delayer::~Delayer(){
    timer_pause(TGROUP,TTIMER);
    timer_set_counter_value(TGROUP,TTIMER, 0);
}

void Delayer::delay(uint32_t ticks){
   uint64_t start = getTicks();
   while (getTicks() - start < ticks) {
       asm("nop");
   }
}

uint64_t Delayer::getTicks(){
    uint64_t ticks = 0;
    timer_get_counter_value(TGROUP,TTIMER,&ticks);
    return ticks;
}