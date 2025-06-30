/*Siddharth Vasudevan
Final_Build/main.c

Contains complete build

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
#include "lib/drv8825.h"


int main() {

    uart_init();

    return 0;
}