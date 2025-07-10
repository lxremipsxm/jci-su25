/*
 *  drv8825.h
 *
 *  Created on: Apr 11, 2024
 *  Author: 	Adriel Luiz Marques
 *  E-mail: 	adrielluizmarques@hotmail.com
 *  LinkedIn: 	https://www.linkedin.com/in/adriel-marques/
 *  GitHub: 	https://github.com/adrielluiz
 *
 */


#ifndef DRV8825_H_
#define DRV8825_H_

typedef enum drv8825_dir_e //rotation direction
{
	DRV8825_DIR_FORWARD = 0,
	DRV8825_DIR_BACKWARD,
} drv8825_dir_t;

typedef enum drv8825_drv_pins_e //Pin organization
{
	/* OUT */  DRV8825_DRV_EN_PIN=7,
	/* OUT */ DRV8825_DRV_M0_PIN=6,
	/* OUT */ DRV8825_DRV_M1_PIN=5,
	/* OUT */ DRV8825_DRV_M2_PIN=4,
	/* OUT */ DRV8825_DRV_RST_PIN=3,
	/* OUT */ DRV8825_DRV_SLP_PIN=2,
	/* OUT */ DRV8825_DRV_STEP_PIN=1,
	/* OUT */ DRV8825_DRV_DIR_PIN=0,

} drv8825_drv_pins_t;

typedef struct drv8825_drv_s //extern function struct
{
	void (*pin_set)(drv8825_drv_pins_t pin, bool state);
	void (*delay_us)(uint32_t t_us);
} drv8825_drv_t;

typedef struct drv8825_scr_s //stepper motor settings
{
	uint16_t steps_per_revolution;
	uint8_t microsteps;
	uint16_t rpm;
} drv8825_scr_t;

void drv8825_set_microsteps(uint8_t microsteps); //set microsteps; takes integer, options: {1,2,4,8,16,32}
void drv8825_set_steps_per_revolution(uint16_t steps_per_revolution); //set number of steps per revolution
void drv8825_set_rpm(uint16_t rpm); 

uint8_t drv8825_get_microsteps(void); //getters
uint16_t drv8825_get_steps_per_revolution(void);
uint16_t drv8825_get_rpm(void);

void drv8825_set_rst_pin(bool state); //self-explanatory
void drv8825_set_slp_pin(bool state);
//void drv8825_set_en_pin(bool state);
void drv8825_set_microsteps_pin(uint8_t microsteps);
void drv8825_move_steps(uint16_t steps, drv8825_dir_t dir);
void drv8825_move_revs(uint16_t revs, drv8825_dir_t dir); //move a certain number of revolutions
bool drv8825_init(drv8825_drv_t *drv, drv8825_scr_t scr);

#endif /* DRV8825_H_ */