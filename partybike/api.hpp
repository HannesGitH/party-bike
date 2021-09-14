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
    bool locked = true;
    BluetoothSerial SerialBT;
    void custom();
    
    // std::map<char, EffectWithArg> possible_effects;
    void effectsrun(String effectstr);
    void sendBuffer(irgb_t buffer[LENGTH_TOTAL]);
public:
    Api(String name);
    ~Api();
    void run();
};

#endif // API_HPP
