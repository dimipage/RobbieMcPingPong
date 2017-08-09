/*
 * led.h
 *
 *  Created on: Aug 1, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef LED_H_
#define LED_H_

#include "gpio.h"

void LED_Init();
void LED_SetBlinkInterval(uint16_t time);
void LED_Update();
void LED_LightOn();
void LED_LightOff();

#endif /* LED_H_ */
