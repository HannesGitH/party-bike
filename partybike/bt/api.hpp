#if !defined(API_HPP)
#define API_HPP

#include "../misc/rgb.h"
#include "BluetoothSerial.h"
#include "../led_party/stripdata.hpp"

class Api
{
private:
    bool locked = true;
    BluetoothSerial SerialBT;
    void custom();
    void sendBuffer(irgb_t buffer[LENGTH_TOTAL]);
public:
    Api(String name);
    ~Api();
    void run();
};

#endif // API_HPP
