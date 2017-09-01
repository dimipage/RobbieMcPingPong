/*
 * led.c
 *
 *  Created on: Aug 1, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "led.h"
int LED_BLINK_INTERVAL;

/**
 * Setting the GPIO pin for LED
 */
void LED_Init(){
	GPIOInit(GPIOB, GPIO_Pin_13, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Fast_Speed);
}

/**
 * LED toggle interval
 * @param st Changing state
 */
void LED_SetBlinkInterval(uint16_t time){
	LED_BLINK_INTERVAL = time;
}


/**
 * Toggles the LED
 * @param l LED struct
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
