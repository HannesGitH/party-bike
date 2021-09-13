#if !defined(API_HPP)
#define API_HPP

#include "BluetoothSerial.h"

class Api
{
private:
    bool locked = true;
    BluetoothSerial SerialBT;
public:
    Api(String name);
    ~Api();
    void run();
};

#endif // API_HPP
