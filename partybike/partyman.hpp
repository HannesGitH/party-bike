#if !defined(PARTYMAN_HPP)
#define PARTYMAN_HPP

#include "led_strip.h"
#include "effects.hpp"
#include "rgb.h"

struct EffectWithArg {
    Effect * eff;
    void * arg;
};

class Partyman
{
private:
    led_strip_t strips[];
public:
    void runEffects(EffectWithArg effects[]);
    void sendBuffer(irgb_t buffer[LENGTH_TOTAL]);
    Partyman(/* args */);
    ~Partyman();
};

#endif // PARTYMAN_HPP
