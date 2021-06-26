#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/timer.h"
#include "hal/timer_types.h"
#include "soc/rtc.h"
//#include "soc/timer_group_struct.h"
//#include "soc/timer_group_reg.h"

#define TGROUP TIMER_GROUP_1
#define TTIMER TIMER_1


typedef uint32_t irgb_t;

irgb_t iRGB
(
    uint8_t R,
    uint8_t G,
    uint8_t B
);

class Delayer
{
private:
    uint32_t _50ns_factor;
    uint64_t _getTicks();
    Delayer(/* args */);
    ~Delayer();
public:
    static Delayer& get(){
        static Delayer instance;
        return instance;
    }
    Delayer(Delayer const&) = delete;
    void operator=(Delayer const&) = delete;

    uint32_t get_speed();

    void delay(uint32_t ticks);
    void delay_50ns(uint32_t multiplier);
};


class Ws2812
{
private:
    gpio_num_t _data_pin;
    Delayer& _d = Delayer::get();
    irgb_t * _data; uint _length;

public:
    Ws2812(gpio_num_t data_pin);
    ~Ws2812();
    void setLEDs(irgb_t data[], uint length);
    void update();
};

