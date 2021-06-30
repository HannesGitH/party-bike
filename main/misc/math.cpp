#include "math.hpp"
#include "math.h"
#include "freertos/FreeRTOS.h"

//GCD
uint32_t ggT(uint32_t a, uint32_t b){
    if(b == 0)
        return a;
    else return ggT(b, a % b);
}

//LCD
uint32_t kgV(uint32_t a, uint32_t b){
    return (a * b) / ggT(a, b);
}