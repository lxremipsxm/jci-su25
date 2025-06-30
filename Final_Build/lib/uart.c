/*Siddharth Vasudevan
lib/uart.c

UART library programmed for 16MHz clock ATMega2560

Comments
--------
6/30/25: Modified for efficiency and ATMega2560
*/
#include <avr/io.h>
#include "uart.h"

// Initialize the UART
void uart_init(void){
    UCSR0B = (1 << TXEN0);                  //enable the UART transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //set 8 bit character size


    UBRR0L = 103;     
    UBBR0H = 0;                      //set baud rate to 9600 for 16 MHz crystal
}
 
// Send a single character
void uart_send_char(unsigned char ch){
    while (!(UCSR0A & (1 << UDRE0)));       //wait til TX data buffer empty
    UDR0 = ch;                              //write the character to the USART data register
}
 
// Send a string of characters using uart_send
void uart_send_string(char *stringAddress){
    while (*stringAddress){
        uart_send_char(*stringAddress++);
    }
}