/*
 * stepper.h
 *
 *  Created on: Aug 4, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "stm32f4xx_tim.h"
#include "gpio.h"

void Step_Init();
void Step_Step();
void Step_Stepback();
void Step_Sequence();



#endif /* STEPPER_H_ */
