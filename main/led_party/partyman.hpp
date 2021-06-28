#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "ws2812_controller.hpp"
#include "rgb.h"

#ifndef PARTYMAN_H
#define PARTYMAN_H

#define DATA_PIN_MAIN GPIO_NUM_13 //main60 //draws way to much power so esp overheats and crashes usb connection
#define LENGTH_MAIN_L 30U
#define LENGTH_MAIN_R 30U
#define LENGTH_REAR_T 4U
#define LENGTH_MAIN (LENGTH_MAIN_L+LENGTH_MAIN_L+LENGTH_REAR_T)

#define DATA_PIN_DIAG GPIO_NUM_12 //diagonal
#define LENGTH_DIAG_L 15U
#define LENGTH_DIAG_R 15U
#define LENGTH_DIAG (LENGTH_DIAG_L+LENGTH_DIAG_L)

#define DATA_PIN_FRNT GPIO_NUM_14 //front
#define LENGTH_FRNT_T 4U
#define LENGTH_FRNT_B 15U
#define LENGTH_FRNT (LENGTH_FRNT_T+LENGTH_FRNT_B)

#define DATA_PIN_SDDL GPIO_NUM_27 //sattelstange
#define LENGTH_SDDL 10U

#define DATA_PIN_REAR GPIO_NUM_23 //rear   //leider is det kabel schrotti, deshalb n anderer pin, wär eigtl 26 (todo)
#define LENGTH_REAR_B 10U


class Partyman
{
private:
    Ws2812_controller stripe_MAIN;

public:
    Partyman(/* args */);
    ~Partyman();
    void walk_pixels();
};

#endif