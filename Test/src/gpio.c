/*
 * gpio.c
 *
 *  Created on: Jul 27, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "gpio.h"


/**
 * Inicijalizacija pina za GPIO
 * @param GPIOx Port pina
 * @param GPIOPin Redni broj pina
 * @param GPIOMode Enumeracija moda pina
 * @param GPIOOType Enumeracija tipa pina
 * @param GPIOPuPd Enumeracija PuPd
 * @param GPIOSpeed Enumeracija brzine (low, mid, fast, high)
*/
GPIO_InitTypeDef GPIOInit(GPIO_TypeDef* GPIOx, uint32_t GPIOPin, GPIOMode GPIOMode, GPIOOType GPIOOType, GPIOPuPd GPIOPuPd, GPIOSpeed GPIOSpeed){
				//		^ probaj enum ovde
	GPIO_InitTypeDef pin;
	pin.GPIO_Mode = GPIOMode;
	pin.GPIO_OType = GPIOOType;
	pin.GPIO_Pin = GPIOPin;
	pin.GPIO_PuPd = GPIOPuPd;
	pin.GPIO_Speed = GPIOSpeed;
	GPIO_Init(GPIOx, &pin);
	return pin;
}

void GPIODeInit(GPIO_InitTypeDef* p){

}
