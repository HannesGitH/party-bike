#include "ws2812.hpp"
#define MEGA 1000000

irgb_t iRGB
(
    uint8_t R,
    uint8_t G,
    uint8_t B
){
    return (uint32_t)R<<16 | (uint32_t)G<<8 | (uint32_t)B;
}


Ws2812::Ws2812(gpio_num_t data_pin)
{
    _data_pin = data_pin;
    gpio_set_direction(_data_pin, GPIO_MODE_OUTPUT);
}
Ws2812::~Ws2812()
{
    //delete &d;
}

void Ws2812::setLEDs(irgb_t data[], uint length){
    _data = data; _length = length;
}

void Ws2812::update(){
    uint64_t start = 0;
    uint64_t end = 0;
    //timer_get_counter_value(TGROUP,TTIMER,&start);
    for (int i = 0; i < _length; ++i) {
        irgb_t IRGB = _data[i];
        uint32_t IGRB = (IRGB & 0x0000FF00) | (IRGB & 0x00FF0000) | (IRGB & 0x000000FF);
        for (uint8_t bit_index=0; bit_index<24; ++bit_index){
            bool bit = (IGRB & 0x800000)!=0;
            
            timer_get_counter_value(TGROUP,TTIMER,&start);
            for (uint16_t i = 0; i < 1000; i++)
            {
                _d.delay_50ns(0);
            }
            timer_get_counter_value(TGROUP,TTIMER,&end);
            gpio_set_level(_data_pin, 1);
            _d.delay_50ns(8*(bit + 1));
            gpio_set_level(_data_pin, 1);
            _d.delay_50ns(8*(2-bit));
        }
    }
    //timer_get_counter_value(TGROUP,TTIMER,&end);
    printf("measurement took %llu 40Mhz cycles",(end-start)/1000);
    _d.delay_50ns(1001);
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
    printf("timer initialized\n");

    //TODO: disable watchdog
    //TIMERG1.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    //TIMERG1.wdt_feed=1;
    //TIMERG1.wdt_wprotect=0;

    _50ns_factor = ((get_speed()>>2)/MEGA);
}

uint32_t Delayer::get_speed(){
    return rtc_clk_apb_freq_get();
}

Delayer::~Delayer(){
    timer_pause(TGROUP,TTIMER);
    timer_set_counter_value(TGROUP,TTIMER, 0);
}

void Delayer::delay(uint32_t ticks){
   uint64_t start = _getTicks();
   while (_getTicks() - start < ticks) {
       asm("nop");
   }
}
void Delayer::delay_50ns(uint32_t multiplier){
    //printf("delaying %d ticks\n",multiplier*((get_speed()>>2)/MEGA));//serial is way to sloooow
    delay(multiplier*_50ns_factor);
}

uint64_t Delayer::_getTicks(){
    uint64_t ticks = 0;
    timer_get_counter_value(TGROUP,TTIMER,&ticks);
    return ticks;
}