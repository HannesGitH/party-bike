
#define PASS "passwot"

#include "api.hpp"
#include "rgb.h"

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
    command = command.substring(0,command.length()-2);
    
    if(command == PASS){
        locked = false;
        SerialBT.println("unlocked");
        return;
    }
    if (locked)return;

    pm.stopLoop();

    if      (command                == "lock"       ) locked = true;
    else if (command                == "custom"     ) custom();
    else if (command.substring(0,7) == "effects"    ) effectsrun(command.substring(8),false);
    else if (command.substring(0,4) == "loop"       ) effectsrun(command.substring(5),true);
    else if (command                == "test"       ) pm.test();
    else if (command                == "reset"      ) pm.reset();

    else SerialBT.printf("\"%s\" not supported\n",command);
  }
  return; 
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
    sendBuffer(buffer);
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
void Api::sendBuffer(irgb_t * buffer){
  pm.sendBuffer(buffer);
}

#include <map>
#include "effects.hpp"

void Api::effectsrun(String effectstr, bool loop){
  free(effects_to_run);//maybe a realloc would be better?
  effects_to_run = (EffectWithArg*) malloc(effectstr.length()* sizeof(EffectWithArg)); //Attention, that needs to be freed
  uint8_t i=0;
  uint8_t failed=0;
  for (char c : effectstr)
  {
    Serial.println(c);
    if(false){}
    else if(c=='w'){effects_to_run[i]={.eff = effect_walk_pixel        , .arg = NULL }; }
    ////else if(c=='s'){effects_to_run[i]={.eff = effect_spread_pixel      , .arg = NULL }; }
    else if(c=='W'){effects_to_run[i]={.eff = effect_walking_colorline , .arg = NULL }; }
    else if(c=='S'){effects_to_run[i]={.eff = effect_streetlight       , .arg = NULL }; }
    else if(c=='r'){effects_to_run[i]={.eff = effect_init_rainbow      , .arg = NULL }; }
    else if(c=='h'){effects_to_run[i]={.eff = effect_change_hue        , .arg = NULL }; }
    else{failed++;}
    i++;
  }
  if(loop)
    pm.loopEffects(effects_to_run,i-failed);//freeing the array has to occur when task is killed;
  else
    pm.runEffects(effects_to_run,i-failed);
  return;
}
