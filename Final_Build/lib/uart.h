/*Siddharth Vasudevan
lib/uart.h

Header file for uart.c

Comments:
6/30/25: copied file over 
*/

#ifndef UART_H

#define UART_H

#include <string.h>

void uart_init(void);
void uart_send_char(unsigned char ch);
void uart_send_string(char *stringAddress);

#endif