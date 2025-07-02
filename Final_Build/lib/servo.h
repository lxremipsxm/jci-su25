/*Siddharth Vasudevan
lib/servo.h

Header file for servo.c
*/

#include <avr/io.h>

void servo_start_pwm();
void servo_set_pos(char servo, int deg);
void servo_stop_pwm();
