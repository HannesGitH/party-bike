
#define PASS "123454321"

#include "api.hpp"

Api::Api(String name)
{
    SerialBT.begin(name);
}

Api::~Api()
{
}

void Api::run(){
    
  if (SerialBT.available()) {
    String command = SerialBT.readString();
    
    if(command == PASS){
        locked = false;
        SerialBT.println("unlocked");
        return;
    }
    if (locked)return;

    if (command == "lock"){
        locked = true;
    }
    else if (command == "party")
    {
        /* code */
    }
  }
    
};