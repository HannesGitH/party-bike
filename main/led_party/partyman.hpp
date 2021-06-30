#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
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




#define amount_strips 5
enum strip_enum{
    MAIN=0,
    DIAG=1,
    FRNT=2,
    SDDL=3,
    REAR=4
};
const uint16_t strip_lengths[] = {
    LENGTH_MAIN,
    LENGTH_DIAG,
    LENGTH_FRNT,
    LENGTH_SDDL,
    LENGTH_REAR_B
};
const gpio_num_t strip_pins[] = {
    DATA_PIN_MAIN,
    DATA_PIN_DIAG,
    DATA_PIN_FRNT,
    DATA_PIN_SDDL,
    DATA_PIN_REAR
};




#endif