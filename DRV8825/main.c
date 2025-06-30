/*Siddharth Vasudevan
DRV8825/main.c

Testing DRV8825

Comments
--------
6/12/25: Created file, copying in recommended structure from https://github.com/adrielluiz
*/


//-----Includes-----//
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/uart.h"
#include "lib/servo.h"
#include "lib/drv8825.h"



/*/-----Pin Mapping-----//
static uint32_t stepper_pin_mapping[8] =
{
    HW_MOTOR_STEP,
    HW_MOTOR_DIR,
    HW_MOTOR_RST,
    HW_MOTOR_SLP,
    HW_MOTOR_EN,
    HW_MOTOR_M0,
    HW_MOTOR_M1,
    HW_MOTOR_M2
};*/


static void stepper_pin_set(drv8825_drv_pins_t pin, bool state)
{
    //drv8825_drv_pins_t is an enum of ints. 
    //I will be using PD0-PD7 to control the stepper motor, so 
    //I can just map the pins directly to their own number.

    if ((DDRD & (1<<pin)) == 0){
        DDRD |= (1<<pin);
    }

    if (state) {
        PORTD |= (1<<pin);
    }
    else {
        PORTD &= ~(1<<pin);
    }
    

}

static void stepper_delay_us(uint32_t t_us)
{
    _delay_us(t_us);
}


//-----Driver config-----//
static drv8825_drv_t drv =
{
    .pin_set = stepper_pin_set,
    .delay_us = stepper_delay_us,
};

//-----Driver Settings-----//
drv8825_scr_t scr =
{
    .steps_per_revolution = 200,
    .microsteps = 1,
    .rpm = 200,
};

//-----Main method-----//
int main(void)
{
    drv8825_init(&drv, scr);
    uart_send_string("Stepper driver initialized.");
    uart_init();

    while(1){
        drv8825_move_steps(200, DRV8825_DIR_FORWARD);
        uart_send_string("Forward");

        stepper_delay_us(1000000);
        
        
    }

    return 0;
}
