#if !defined(PARTYMAN_HPP)
#define PARTYMAN_HPP

#include "../led_strip/led_strip.h"
#include "effects.hpp"
#include "../misc/rgb.h"

struct effectWithArg {
    effect * effect;
    void * arg;
};

class Partyman
{
private:
    led_strip_t strips[];
public:
    void runEffects(effectWithArg effects[]);
    void sendBuffer(irgb_t buffer[LENGTH_TOTAL]);
    Partyman(/* args */);
    ~Partyman();
};

#endif // PARTYMAN_HPP
