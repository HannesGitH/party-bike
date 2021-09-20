#if !defined(PARTYMAN_HPP)
#define PARTYMAN_HPP

#include "led_strip.h"
#include "effects.hpp"
#include "rgb.h"
#include <Arduino.h>

struct EffectWithArg {
    Effect eff;
    void * arg;
};

class Partyman
{
private:
    led_strip_t strips[amount_strips];

    xSemaphoreHandle * mutexies;

    irgb_t * fullbuf;

    xTaskHandle loopHandle;

    bool init_new_strip(led_strip_t * led_strip_p, gpio_num_t strip_data_pin, uint length, uint8_t channel);
    void initialize_strips(led_strip_t * strips);
    void led_strips_clear(led_strip_t * strips);
public:
    uint speed;
    void test();
    void reset();
    void runEffects(EffectWithArg effects[],uint8_t len);
    void loopEffects(EffectWithArg effects[],uint8_t len);
    void driveEffects(Effect * effects, uint8_t amount, void * extra_args_p[], bool run_all_in_every_step);
    void stopLoop();
    void sendBuffer(irgb_t buffer[LENGTH_TOTAL]);
    Partyman();
    ~Partyman();
};

#endif // PARTYMAN_HPP
