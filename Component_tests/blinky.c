/*Siddharth Vasudevan
jci-su25/Component_tests/blinky.c

Intended for checking integrity of current arduino board

Comments
--------
6/2/25: Included uart library, functionality verified

*/


//-----Includes-----//
#include <avr/io.h>
#include <stdlib.h>
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


    /*Testing PWM*/
    while(1){

        char output[32];
    }
    
    return 0;
}

