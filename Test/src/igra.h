/*
 * igra.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef IGRA_H_
#define IGRA_H_

#include "dequeue.h"
#include "stepper.h"
#include "button.h"
#include "led.h"
#include "gpio.h"
#include "pwm.h"
#include "DCMotor.h"
#include "RCServo.h"
#include "seg.h"

void State0(); //init 1
void State1();
void State2(); //start_pauza flag?
void State3(); //init 0



#endif /* IGRA_H_ */
