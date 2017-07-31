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
	SPEED_HIGH = 4000, //dodaj vrednosti posle
	SPEED_MID = 2000,
	SPEED_LOW = 500
}DC_Speed;

typedef enum {
	MOT1,
	MOT2,
	MOT3
}DC_Motor;

void DC_SetSpeed(DC_Motor motor, DC_Speed speed);
void DC_Init();

#endif /* DCMOTOR_H_ */
