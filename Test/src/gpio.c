/*
 * gpio.c
 *
 *  Created on: Jul 27, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "gpio.h"


/**
 * Init GPIO pin
 * @param GPIOx Pin port
 * @param GPIOPin Pin number
 * @param GPIOMode Pin mode enum
 * @param GPIOOType Pin type enum
 * @param GPIOPuPd PuPd enum
 * @param GPIOSpeed Speed enum (low, mid, fast, high) 						//too general; depricated
*/
GPIO_InitTypeDef GPIOInit(GPIO_TypeDef* GPIOx, uint32_t GPIOPin, GPIOMode GPIOMode, GPIOOType GPIOOType, GPIOPuPd GPIOPuPd, GPIOSpeed GPIOSpeed){
	GPIO_InitTypeDef pin;
	pin.GPIO_Mode = GPIOMode;
	pin.GPIO_OType = GPIOOType;
	pin.GPIO_Pin = GPIOPin;
	pin.GPIO_PuPd = GPIOPuPd;
	pin.GPIO_Speed = GPIOSpeed;
	GPIO_Init(GPIOx, &pin);
	return pin;
}
/**
 * Deinicijalizacija pina GPIO
 * @param p
 */
void GPIODeInit(GPIO_InitTypeDef* p){

}
