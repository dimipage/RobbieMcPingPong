/*
 * DCMotor.h
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "gpio.h"
#include "queue.h"

typedef enum {
	SPEED_EASY = 40,
	SPEED_MEDIUM = 45,
	SPEED_HARD = 50,
	SPEED_BAJCHA = 65
}DC_Speed;

typedef enum {
	MOT1,
	MOT2,
	MOT3
}DC_Motor;

void DC_SetSpeed(DC_Motor motor,DC_Speed speed);	//test

void DC_SetBotSpeed(Ball_typedef* ball); //bottom fields DC speed
void DC_SetTopSpeed(Ball_typedef* ball); //top fields DC speed

void DC_DeInit(); // ne zaboravi da definises
void DC_Init();
#endif /* DCMOTOR_H_ */
