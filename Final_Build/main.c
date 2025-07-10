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
#include <stdlib.h>

//-----Libs-----//
#include "lib/uart.h"
#include "lib/servo.h"
#include "lib/drv8825_2560.h"


//-----Stepper Declarations-----//
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


//Driver config
static drv8825_drv_t drv =
{
    .pin_set = stepper_pin_set,
    .delay_us = stepper_delay_us,
};

//Driver Settings
drv8825_scr_t scr =
{
    .steps_per_revolution = 200,
    .microsteps = 1,
    .rpm = 200,
};

void stepper_move_to_pos(int pos, char *current_pos){


    if (pos > 16) 
        pos = 16;

    if (pos < 1)
        pos = 1;

    signed int diff = pos- *current_pos; 

    if (diff > 0){

        drv8825_move_steps(401*diff, DRV8825_DIR_FORWARD);
    } 

    else if (diff < 0){

        drv8825_move_steps(401*-diff, DRV8825_DIR_BACKWARD);
    }

    *current_pos = pos;

}


//-----Servo Declarations-----//
void servo_raise(int servo){
    servo_set_pos(servo,50);
    _delay_ms(400);
    servo_set_pos(servo,0);
    _delay_ms(100);

}

//-----Keypad declarations-----//
const char keymap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypad_init(){
    //Using Port A for keypad
    DDRA = 0xF0; //first nibble for rows(output), second for columns (input)
    PORTA = 0xFF; //Pulls inputs HIGH and outputs 5V at outputs
}

char wait_for_instruction(){
    for (uint8_t row = 0; row < 4; row++) {
        // Set all rows HIGH
        PORTA |= 0xF0;

        // Drive current row LOW
        PORTA &= ~(1 << (row + 4));

        _delay_us(5);

        // Read column inputs 
        uint8_t col_input = PINA & 0x0F;

        // Check each column for LOW
        for (uint8_t col = 0; col < 4; col++) {
            if (!(col_input & (1 << col))) {

                return keymap[row][col];
            }
        }
    }

    return 0;

}



//-----Main method-----//
int main(void)
{

    //Setup
    uart_init();
    drv8825_init(&drv, scr);
    servo_start_pwm();
    keypad_init();


    //Stepper Settings
    drv8825_set_steps_per_revolution(200);
    drv8825_set_microsteps(32);
    drv8825_set_microsteps_pin(32);
    unsigned char current_pos = 1; // 1 to 16 for our purposes

    _delay_ms(1000);
    

    while(1){

        
        
    }

    return 0;
}
