/*
 * led.h
 *
 *  Created on: Aug 1, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef LED_H_
#define LED_H_

#include "gpio.h"

typedef enum{
	SYSTEM_NON_INIT,
	SYSTEM_START,
	SYSTEM_OUT_OF_BALLS
} LED_STATE;
typedef struct{
	LED_STATE state;
	uint16_t pin;
	GPIO_TypeDef* GPIOx;
	uint16_t counter;
}LED_InitType;

LED_InitType LED_Init(LED_STATE state, uint16_t pin, GPIO_TypeDef* GPIOx);
void LED_SetState(LED_InitType* led, LED_STATE st);
void LED_Update(LED_InitType* l);


#endif /* LED_H_ */
