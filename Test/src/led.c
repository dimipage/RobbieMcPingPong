/*
 * led.c
 *
 *  Created on: Aug 1, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "led.h"
/**
 *
 * @param state
 * @param pin
 */
LED_InitType LED_Init(LED_STATE st, uint16_t p, GPIO_TypeDef* x){
	LED_InitType led;
	InitGPIO(x, p, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Fast_Speed);
	led.pin = p;
	led.GPIOx = x;
	LED_SetState(&led, st);
	return led;
}
/**
 *
 * @param led
 * @param st
 */
void LED_SetState(LED_InitType* led, LED_STATE st){
	led->state = st;
	switch(st){
	case SYSTEM_NON_INIT:
		led->counter = 1000;
		break;
	case SYSTEM_OUT_OF_BALLS:
		led->counter = 500;
		break;
	case SYSTEM_START:
		led->counter = -1;
		GPIO_ResetBits(led->GPIOx, led->pin);
		break;
	}
}
/**
 *
 * @param l
 */
void LED_Update(LED_InitType* l){
	extern ticks;
	if(ticks % l->counter == 0)
		GPIO_ToggleBits(l->GPIOx, l->pin);

}

