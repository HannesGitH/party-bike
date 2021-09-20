#if !defined(API_HPP)
#define API_HPP

#include "rgb.h"
#include "BluetoothSerial.h"
#include "stripdata.hpp"
#include "partyman.hpp"

class Api
{
private:
    Partyman pm = Partyman();
    EffectWithArg * effects_to_run; // so we can alloc and free memory for it (needs to be persistent for other tasks to access it (e.g. in loopeffects))
    bool locked = true;
    BluetoothSerial SerialBT;
    void custom();
    
    // std::map<char, EffectWithArg> possible_effects;
    void effectsrun(String effectstr, bool loop);
    void sendBuffer(irgb_t buffer[LENGTH_TOTAL]);
public:
    Api(String name);
    ~Api();
    void run();
};

#endif // API_HPP
