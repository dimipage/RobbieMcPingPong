/*
 * led.c
 *
 *  Created on: Aug 1, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "led.h"
int LED_BLINK_INTERVAL;

/**
 *
 */
void LED_Init(){
	GPIOInit(GPIOB, GPIO_Pin_13, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Fast_Speed);
}

/**
 * Menjanje stanja diode. To ce uticati na duzinu trajanja treperenja
 * @param led Struktura LED kojoj se menja stanje
 * @param st Stanje u koje se menja
 */
void LED_SetBlinkInterval(uint16_t time){
	LED_BLINK_INTERVAL = time;
}
/**
 * Toggle-uje diodu ako je proslo dovoljno vremena
 * @param l Struktura LED koja se toggluje
 */
void LED_Update(){
	extern ticks;
	if(ticks % LED_BLINK_INTERVAL == 0)	//kako sam ja zamislio ovo da radi?
		GPIO_ToggleBits(GPIOB, GPIO_Pin_13);
}

void LED_LightOn(){
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
}


void LED_LightOff(){
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);
}
