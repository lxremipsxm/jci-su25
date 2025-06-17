/*
 *  drv8825.c
 *
 *  Created on: Apr 11, 2024
 *  Author: 	Adriel Luiz Marques
 *  E-mail: 	adrielluizmarques@hotmail.com
 *  LinkedIn: 	https://www.linkedin.com/in/adriel-marques/
 *  GitHub: 	https://github.com/adrielluiz
 *
 */

/*

Modified by Siddharth Vasudevan


Comments
--------
6/12/25: Removed all misspellings of drv8825 in function and type names
6/17/25: Adding comments to the header file.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "drv8825.h"

typedef struct drv8825_drv_data_s
{
	drv8825_drv_t drv;
	drv8825_scr_t scr;
} drv8825_drv_data_t;

static drv8825_drv_data_t drv_data = { 0 };

void drv8825_set_microsteps(uint8_t microsteps)
{
	drv_data.scr.microsteps = microsteps;
}

void drv8825_set_steps_per_revolution(uint16_t steps_per_revolution)
{
	drv_data.scr.steps_per_revolution = steps_per_revolution;
}

void drv8825_set_rpm(uint16_t rpm)
{
	drv_data.scr.rpm = rpm;
}

uint8_t drv8825_get_microsteps(void)
{
	return drv_data.scr.microsteps;
}

uint16_t drv8825_get_steps_per_revolution(void)
{
	return drv_data.scr.steps_per_revolution;
}

uint16_t drv8825_get_rpm(void)
{
	return drv_data.scr.rpm;
}

void drv8825_set_rst_pin(bool state)
{
	drv_data.drv.pin_set(DRV8825_DRV_RST_PIN, state);
}

void drv8825_set_slp_pin(bool state)
{
	drv_data.drv.pin_set(DRV8825_DRV_SLP_PIN, state);
}

void drv8825_set_en_pin(bool state)
{
	drv_data.drv.pin_set(DRV8825_DRV_EN_PIN, state);
}

void drv8825_set_microsteps_pin(uint8_t microsteps)
{
	switch (microsteps)
	{
	case 1:
		drv_data.drv.pin_set(DRV8825_DRV_M0_PIN, false);
		drv_data.drv.pin_set(DRV8825_DRV_M1_PIN, false);
		drv_data.drv.pin_set(DRV8825_DRV_M2_PIN, false);
		break;
	case 2:
		drv_data.drv.pin_set(DRV8825_DRV_M0_PIN, true);
		drv_data.drv.pin_set(DRV8825_DRV_M1_PIN, false);
		drv_data.drv.pin_set(DRV8825_DRV_M2_PIN, false);
		break;
	case 4:
		drv_data.drv.pin_set(DRV8825_DRV_M0_PIN, false);
		drv_data.drv.pin_set(DRV8825_DRV_M1_PIN, true);
		drv_data.drv.pin_set(DRV8825_DRV_M2_PIN, false);
		break;
	case 8:
		drv_data.drv.pin_set(DRV8825_DRV_M0_PIN, true);
		drv_data.drv.pin_set(DRV8825_DRV_M1_PIN, true);
		drv_data.drv.pin_set(DRV8825_DRV_M2_PIN, false);
		break;
	case 16:
		drv_data.drv.pin_set(DRV8825_DRV_M0_PIN, false);
		drv_data.drv.pin_set(DRV8825_DRV_M1_PIN, false);
		drv_data.drv.pin_set(DRV8825_DRV_M2_PIN, true);
		break;
	case 32:
		drv_data.drv.pin_set(DRV8825_DRV_M0_PIN, true);
		drv_data.drv.pin_set(DRV8825_DRV_M1_PIN, true);
		drv_data.drv.pin_set(DRV8825_DRV_M2_PIN, true);
		break;
	default:
		break;
	}
}

void drv8825_move_steps(uint16_t steps, drv8825_dir_t dir)
{
    if (dir == DRV8825_DIR_FORWARD)
    	drv_data.drv.pin_set(DRV8825_DRV_DIR_PIN, false);
    else
		drv_data.drv.pin_set(DRV8825_DRV_DIR_PIN, true);

    uint16_t pulse_time_us = (60.0*1000000L/steps/drv_data.scr.microsteps/drv_data.scr.rpm);

    for (uint16_t i = 0; i < steps; i++)
    {
    	drv_data.drv.pin_set(DRV8825_DRV_STEP_PIN, true);
    	drv_data.drv.delay_us(pulse_time_us);
    	drv_data.drv.pin_set(DRV8825_DRV_STEP_PIN, false);
    	drv_data.drv.delay_us(pulse_time_us);
    }
}

void drv8825_move_revs(uint16_t revs, drv8825_dir_t dir)
{
	drv8825_move_steps(revs * (drv_data.scr.steps_per_revolution * drv_data.scr.microsteps), dir);
}

bool drv8825_init(drv8825_drv_t *drv, drv8825_scr_t scr)
{
	if(drv == NULL)
		return false;

	drv_data.drv = *drv;
	drv_data.scr = scr;

	drv8825_set_microsteps_pin(scr.microsteps);

	return true;
}