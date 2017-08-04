/*
 * DCMotor.h
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "pwm.h"
#include "gpio.h"

typedef enum {
	SPEED_EASY = 3000,
	SPEED_MEDIUM = 3500,
	SPEED_HARD = 4000,
	SPEED_BAJCHA = 5080
}DC_Speed;

typedef enum {
	MOT1,
	MOT2,
	MOT3
}DC_Motor;

void DC_SetSpeed(DC_Motor motor, DC_Speed speed);
void DC_Init();

#endif /* DCMOTOR_H_ */
