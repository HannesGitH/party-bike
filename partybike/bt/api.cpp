
#define PASS "123454321"

#include "api.hpp"
#include "../misc/rgb.h"

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

    if (command == "lock")locked = true;
    else if (command == "custom")custom();
  } 
};

void Api::custom(){
  bool nonStop = true;
  irgb_t * buffer = (irgb_t *) malloc(sizeof(irgb_t)*LENGTH_TOTAL);
  uint8_t nextVal[4];
  while(nonStop)
  {
    for (uint16_t i = 0; i < LENGTH_TOTAL; i++)
    {
      SerialBT.readBytes(nextVal,4);
      irgb_t next = fromarray(nextVal);
      if(next.i == 0xFD)continue;       //skip this led
      if(next.i == 0xFE)break;          //restart led_strip
      if(next.i == 0xFF)nonStop=false;  //custom animation end
      buffer[i] = next;
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}