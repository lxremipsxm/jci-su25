/*Siddharth Vasudevan
lib/servo.c

Intended for easy servo implementation

Comments
--------
6/3/25: Created file from Component_tests/blinky.c
6/4/25: I will need to make this library extensible, since I want it to be easy to use
6/12/25: modified some comments
*/

#include "servo.h"

void servo_start_pwm(){ //Sets up pwm servo control on PB1
    DDRB |= (1<<1); //Enable PB1 output

    TCCR1A |= (1<<COM1A1); //flag that sets output to low as soon as timer count reaches the compare value
    //also called non-inverting mode

    //WGM11 to WGM14 stands for waveform generation. The current selection, 1110, is FAST PWM mode, where TOP=ICR1. TOP is the maximum
    //value the timer can reach before it restarts, and controls the frequency of the pwm. ICR1 is input capture register, which is what we 
    //set TOP to. In this case, TOP = 40000 counts
    TCCR1B |= (1<<WGM13);
    TCCR1B |= (1<<WGM12);
    TCCR1A |= (1<<WGM11);

    TCCR1B |= (1<<CS11); //set prescaler to /8, enable timer


    ICR1 = 39999; //0 to 40000 with our clock is 50Hz
    OCR1A = 1000; //set servo to 0 degrees initially

    //Overall, this produces a pwm where the output goes HIGH at 0, goes low at 3999, and resets at 39999, to give us a 50Hz square wave 
    //with 10% duty cycle
}

void servo_set_pos(int deg){ 
    
    //pwidth is pulse width. In essence, it modifies the duty cycle of the servo.
    //Its range is 1000 us to 5000 us. If outside this range, the servo will not move.
    
    double pwidth = ((deg/180.0) * 2000.0) + 500.0; //maps pwidth to 0 to 180 degrees
    pwidth = (int)pwidth; //cast to int

    if (pwidth > 2500){
        pwidth = 2500;
    }
    
    if (pwidth < 500){
        pwidth = 500;
    }

    OCR1A = pwidth*2; 
}

void servo_stop_pwm(){
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); //stop pwm
}
