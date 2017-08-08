/*
 * led.c
 *
 *  Created on: Aug 1, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "led.h"

/**
 * Inicijalizacija pina za LED
 * @param st Enumeracija koja ukazuje na stanje LED tj vreme treperenja
 * @param p Pin za LED
 * @param x Port za LED
 * @return Struktura LED za jednostavniju upotrebu u ostalim funkcijama
 */

LED_InitType LED_Init(LED_STATE st, uint16_t p, GPIO_TypeDef* x){
	LED_InitType led;
	GPIOInit(x, p, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Fast_Speed);
	led.pin = p;
	led.GPIOx = x;
	LED_SetState(&led, st);
	return led;
}
/**
 * Menjanje stanja diode. To ce uticati na duzinu trajanja treperenja
 * @param led Struktura LED kojoj se menja stanje
 * @param st Stanje u koje se menja
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
	case SYSTEM_TEST:
		led->counter = 1;
	}
}
/**
 * Toggle-uje diodu ako je proslo dovoljno vremena
 * @param l Struktura LED koja se toggluje
 */
void LED_Update(LED_InitType* led){
	extern ticks;
	if(ticks % led->counter == 0)//kako sam ja zamislio ovo da radi?
		GPIO_ToggleBits(led->GPIOx, led->pin);

}

