/*Siddharth Vasudevan
Component_tests/lib/uart.c

UART library programmed for 16MHz clock ATMega328p

Comments:
6/2/25: Created file, note that this code assumes avr/io.h is already included in the main .c file
*/
#include <avr/io.h>
#include "uart.h"

// Initialize the UART
void uart_init(void){
    UCSR0B = (1 << TXEN0);                  //enable the UART transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //set 8 bit character size
    UBRR0L = 103;                           //set baud rate to 9600 for 16 MHz crystal
}
 
// Send a single character
void uart_send_char(unsigned char ch){
    while (!(UCSR0A & (1 << UDRE0)));       //wait til TX data buffer empty
    UDR0 = ch;                              //write the character to the USART data register
}
 
// Send a string of characters using uart_send
void uart_send_string(char *stringAddress){
   unsigned char i;
    for (i = 0; i < strlen(stringAddress); i++)
        uart_send_char(stringAddress[i]);
}