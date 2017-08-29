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
	MOT1,
	MOT2,
	MOT3
}DC_Motor;

void DC_SetSpeed(DC_Motor motor, uint8_t speed);	//test

void DC_SetBotSpeed(Ball_typedef* ball);
void DC_SetTopSpeed(Ball_typedef* ball);

void DC_DeInit(); // ne zaboravi da odradis
void DC_Init();

#endif /* DCMOTOR_H_ */
