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
#include <string.h>

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

void stepper_move_to_pos(int pos, int *current_pos){

    
    if (pos > 16) //hard cap between 1 and 16 (inclusive)
        pos = 16;

    if (pos < 1)
        pos = 1;

    signed int diff = pos - *current_pos; //difference in position
    int movement;


    //if difference greater than 8, swap difference around for shorter path
    if (diff > 8){
        diff = diff-16;
    }

    else if (diff < -8){
        diff = 16+diff;
    }


    if (diff > 0){

        movement = (402*diff); 
        drv8825_move_steps(movement, DRV8825_DIR_FORWARD);
        
    } 

    else if (diff < 0){

        movement = (402*-diff);
        drv8825_move_steps(movement, DRV8825_DIR_BACKWARD);
        
    }

    *current_pos = pos;

}


void stepper_move_card_to_pos(int pos, int reader, int *current_pos){

    //All this does is use the position at reader A to determine the positions of the other three readers,
    //since they have a constant difference of 4 slots between each. I'll create a wrap-around functionality
    //to easily work with the 1-16 cap I have set in stepper_move_to_pos().
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
    DDRA = (1 << PA1) | (1 << PA3) | (1 << PA5) | (1 << PA7); //first nibble for rows(output), second for columns (input)
    PORTA = 0xFF; //Pulls inputs HIGH and outputs 5V at outputs
}

char keypad_char() {
    char key = 0;

    while (key == 0) { //wait until character entered
        for (uint8_t row = 0; row < 4; row++) {
            // Set all rows HIGH
            PORTA |= (1 << PA1) | (1 << PA3) | (1 << PA5) | (1 << PA7);

            // Drive current row LOW
            switch (row) {
                case 0: PORTA &= ~(1 << PA1); break;
                case 1: PORTA &= ~(1 << PA3); break;
                case 2: PORTA &= ~(1 << PA5); break;
                case 3: PORTA &= ~(1 << PA7); break;
            }

            _delay_us(5);

            // Read column inputs 
            uint8_t col_input = 0;
            col_input |= (PINA & (1 << PA6)) ? (1 << 0) : 0; //if pin A6 high, bitwise or col_input with 0x01
            col_input |= (PINA & (1 << PA4)) ? (1 << 1) : 0;
            col_input |= (PINA & (1 << PA0)) ? (1 << 2) : 0;
            col_input |= (PINA & (1 << PA2)) ? (1 << 3) : 0;

            for (uint8_t col = 0; col < 4; col++) {
                if (!(col_input & (1 << col))) {
                    key = keymap[row][col];
                    break;
                }
            }
            if (key) break; //wait until a key is pressed
        }
    }
    return key;
}


void keypad_string(char *stringAddress){

    char response;
    uint8_t len = 0;

    while (1){
        response = keypad_char();
        

        if (response == '*') 
            break;


        if (len < 5){
            stringAddress[len++] = response; //concatenate with previous numbers entered
            stringAddress[len] = '\0'; //add null terminator
        }

        _delay_ms(350);
    }
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
    int current_pos = 1; // 1 to 16 for our purposes

    char code[4];

    _delay_ms(1000);
    

    while(1){
        
        code[0] = '\0'; //clear string  
        
        keypad_string(code);
        int len = strlen(code);

        if (len == 2){
                char temp[4];
                temp[0] = '0';
                temp[1] = '\0'; 
                strcat(temp, code);

                strcpy(code, temp);
                len += 1;
        }

        if (len > 3){
            uart_send_string("Invalid input. Input longer than three characters. \r\n");
        }

        else if ((code[len-1] != 'A') && (code[len-1] != 'B') && (code[len-1] != 'C') && (code[len-1] != 'D')){
            uart_send_string("Invalid input.  Enter a card number followed by the reader you would like it to be read by. E.g. 1A or 01A.\r\n");
        }

        else if ((code[0] != '1' && code[0] != '0') || (code[1] < '0' || code[1] > '9') || (code[0] == '1' && code[1] >= '7')){
            
            uart_send_string("Invalid input. Either your input card number is out of bounds, or you've entered a value with the wrong format. ");
            uart_send_string("Enter a card number followed by the reader you would like it to be read by. E.g. 1A or 01A. \r\n");
        }

        else {

            uart_send_string(code);
            uart_send_string("\r\n");

            char card_str[3];
            card_str[0] = code[0];
            card_str[1] = code[1];
            card_str[2] = '\0';
            int card_int = atoi(card_str);

            /*debug message
            char debug_msg[30];
            sprintf(debug_msg, "current pos before = %d\r\n", current_pos);
            uart_send_string(debug_msg);
            */

            char reader = code[2];

            stepper_move_to_pos(card_int, &current_pos);

            /*debug message 2
            char debug_msg2[32];
            sprintf(debug_msg2, "current pos after = %d\r\n", current_pos);
            uart_send_string(debug_msg2);*/
        
            _delay_ms(200);
            servo_raise(1);

            
            
        }


        _delay_ms(700);
    }

    return 0;

}