/*Siddharth Vasudevan
Component_tests/blinky.c

Intended for checking integrity of current arduino board

Comments
--------
6/2/25: Included uart library, functionality verified
6/2/25: Created test scenario for I/O, NOT tested yet
6/3/25: Tested I/O successfully, testing timers for servo purposes
6/3/25: Servo tested successfully. Will create library files for easy control later.
*/


//-----Includes-----//
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "lib/uart.h"
#include "lib/servo.h"
#include "lib/SSD1306.h"

//-----Main method-----//
int main(void){

    uart_init();
    

    /*Testing basic functionality + uart library
    while (1){
        char string[] = "Testing\r\n";
        uart_send_string(string);
        _delay_ms(500);
    }
    */

    
    /*Testing io
    DDRB |= 1 << 5; //set pin B5 to output
    DDRC &= ~(1 << 5); //set pin C5 to input

    PORTB = 0;
    PORTC = 0;

    //This snippet toggles bit 5 of the portb register. PB5 and PC5 are 
    //connected, so reading PB5 causes it to toggle.
    while(1){
        char output[32];
        
        if((PORTC & 0x20)!= 0){
            PORTB &= ~(1 << 5);
            sprintf(output, "PORTB: %d\r\n", PORTB);
            
        }
        else {
            PORTB |= (1<<5);
            sprintf(output, "PORTB: %d, PORTC: %d\r\n", PORTB, PORTC);
        }
        
        uart_send_string(output);
        _delay_ms(500);
    
    }*/

    /*Testing servos
    servo_start_pwm();

    while(1){
        servo_set_pos(0);
        _delay_ms(1000);
        servo_set_pos(90);
        _delay_ms(1000);
    }*/

    /*OLED Testing*/

    OLED_Init();
    while(1){
        OLED_GoToLine(4);
        OLED_DisplayString("interior");
        _delay_ms(1000);
    }

    
    return 0;
}
