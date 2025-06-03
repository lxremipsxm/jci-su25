/*Siddharth Vasudevan
lib/servo.c

Intended for easy servo implementation
*/

#include "servo.h"

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
