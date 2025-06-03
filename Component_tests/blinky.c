/*Siddharth Vasudevan
jci-su25/Component_tests/blinky.c

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


void servo_start_pwm();
void servo_set_pos(int deg);
void servo_stop_pwm();

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
    servo_pwm_init();

    while(1){
        servo_set_pos(0);
        _delay_ms(1000);
        servo_set_pos(180);
        _delay_ms(1000);
    }

    */
    return 0;
}


void servo_start_pwm(){ //Sets up pwm servo control on PB1
    DDRB |= (1<<1); //Enable PB1 output

    TCCR1A |= (1<<COM1A1); //flag that sets output to low as soon as timer count reaches the compare value
    //also called non-inverting mode

    TCCR1B |= (1<<WGM13);
    TCCR1B |= (1<<WGM12);
    
    TCCR1A |= (1<<WGM11);

    TCCR1B |= (1<<CS11); //set prescaler to /8
    
    //WGM11 to WGM14 stands for waveform generation. The current selection, 1110, is FAST PWM mode, where TOP=ICR1. TOP is the maximum
    //value the timer can reach before it restarts, and controls the frequency of the pwm. ICR1 is input capture register, which is what we 
    //set TOP to

    ICR1 = 39999; //0 to 40000 with our clock is 50Hz
    OCR1A = 3999; //4000, 10% duty cycle, 0 degrees

    //Overall, this produces a pwm where the output goes HIGH at 0, goes low at 3999, and resets at 39999, to give us a 50Hz square wave 
    //with 10% duty cycle
}

void servo_set_pos(int deg){ 
    
    //usec can be from 2*500 us to 2*2500 us, otherwise pulse width is incompatible with servo
    
    double usec = ((deg/180.0) * 2000.0) + 500.0;
    usec = (int)usec;

    if (usec > 2500){
        usec = 2500;
    }
    
    if (usec < 500){
        usec = 500;
    }

    OCR1A = usec*2; 
}

void servo_stop_pwm(){
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); //stop pwm
}
