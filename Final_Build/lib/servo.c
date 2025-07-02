/*Siddharth Vasudevan
lib/servo.c

Intended for easy servo implementation

Comments
--------
6/30/25: Created file
7/1/25: Modifying functionality for Arduino Mega
*/

#include "servo.h"

void servo_start_pwm(){ //Sets up pwm servo control
    DDRB |= 0xE0; //Enable PB5,6,7 output: 0b 1110 0000

    TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<COM1C1); //flag that sets output to low as soon as timer count reaches the compare value
    //also called non-inverting mode

    //WGM11 to WGM14 stands for waveform generation. The current selection, 1110, is FAST PWM mode, where TOP=ICR1. TOP is the maximum
    //value the timer can reach before it restarts, and controls the frequency of the pwm. ICR1 is input capture register, which is what we 
    //set TOP to. In this case, TOP = 40000 counts
    TCCR1B |= (1<<WGM13);
    TCCR1B |= (1<<WGM12);
    TCCR1A |= (1<<WGM11);

    TCCR1B |= (1<<CS11); //set prescaler to /8, enable timer

    ICR1 = 39999; //0 to 40000 with our clock is 50Hz

    OCR1A = 1000; //set servo to 0 degrees initially (5% duty cycle)
    //Mega
    OCR1B = 1000;
    OCR1C = 1000;

    //Overall, this produces a pwm where the output goes HIGH at 0, goes low at 1000, and resets at 39999, to give us a 50Hz square wave 
    //with 5% duty cycle

    //For the 2560, I will enable Timer3 as well in the same mode as Timer1
    DDRE |= 0x08; //set PE3 output: 0b 0000 1000
    TCCR3A |= (1<<COM3A1); //non inverting mode (see timer1 setup above for explanation)
    TCCR3B |= (1<<WGM33) | (1<<WGM32);
    TCCR3A |= (1<<WGM31);

    TCCR3B |= (1<<CS31);

    ICR3 = 39999;
    OCR3A = 1000; //set servo to 0 degrees (5% duty cycle)
}

void servo_set_pos(char servo, int deg){ 
    
    //pwidth is pulse width. In essence, it modifies the duty cycle of the servo.
    //Its range is 1000 us to 5000 us (tested manually). If outside this range, the servo will not move.
    
    double pwidth = ((deg/180.0) * 2000.0) + 500.0; //maps pwidth to 0 to 180 degrees
    pwidth = (int)pwidth; //cast to int


    //pwidth limits
    if (pwidth > 2500){pwidth = 2500;}
    
    if (pwidth < 500){pwidth = 500;}


    //servo number limits
    if (servo > 4) {servo = 4;}

    if (servo < 1) {servo = 1;}


    switch (servo){

        case 1:
            OCR1A = pwidth*2;
            break;
        
        case 2:
            OCR1B = pwidth*2;
            break;

        case 3: 
            OCR1C = pwidth*2;
            break;

        case 4:
            OCR3A = pwidth*2;
            break;
        
        default:
            break;

    }


}

void servo_stop_pwm(){
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); //stop pwm
}
