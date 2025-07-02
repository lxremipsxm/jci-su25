/*Siddharth Vasudevan
Final_Build/main.c

Intended for complete build

Comments
--------
6/30/25: Created file
*/


//-----Includes-----//
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

//-----Libs-----//
#include "lib/uart.h"
#include "lib/servo.h"
#include "lib/drv8825_2560.h"



static void stepper_pin_set(drv8825_drv_pins_t pin, bool state)
{
    //drv8825_drv_pins_t is an enum of ints. 
    //I will be using PF0-PF7 to control the stepper motor, so 
    //I can just map the pins directly to their own number.

    if ((DDRF & (1<<pin)) == 0){
        DDRF |= (1<<pin);
    }

    if (state) {
        PORTF |= (1<<pin);
    }
    else {
        PORTF &= ~(1<<pin);
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
    .rpm = 1,
};

//-----Main method-----//
int main(void)
{
    servo_start_pwm();
    uart_init();
    _delay_ms(1000);

    while(1){
        servo_set_pos(1,90);
        servo_set_pos(2,90);
        servo_set_pos(3,90);
        servo_set_pos(4,90);

        uart_send_string("90 deg\r\n");


        _delay_ms(1000);

        servo_set_pos(1,0);
        servo_set_pos(2,0);
        servo_set_pos(3,0);
        servo_set_pos(4,0);

        uart_send_string("0 deg\r\n");

        _delay_ms(1000);
        
        
    }

    return 0;
}
