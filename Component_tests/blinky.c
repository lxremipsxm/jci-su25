/*Siddharth Vasudevan
jci-su25/Component_tests/blinky.c

Intended for checking integrity of current arduino board

Comments
--------
6/2/25: Included uart library, functionality verified
6/2/25: Created test scenario for I/O, NOT tested yet

*/


//-----Includes-----//
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "lib/uart.h"



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

    
    /*Testing io*/
    DDRB |= 1 << 0x20; //set pin B5 to output
    DDRC &= ~(1 << 0x20); //set pin C5 to input

    PORTB = 0;
    PORTC = 0;

    while(1){
        char output[32];
        
        if((PINC & 0x20)!= 0){
            PORTB &= ~(1 << PB5);
            sprintf(output, "Sent %d", 0);
        }
        else {
            PORTB |= (1<<0x20);
            sprintf(output, "Sent %d", 1);
        }
        
        uart_send_string(output);
        _delay_ms(500);
    
    }
    
    return 0;
}

