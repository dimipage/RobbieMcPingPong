/*
 * gpio.c
 *
 *  Created on: Jul 27, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "gpio.h"
#include "stm32f4xx_rcc.h"

/**
 * GPIO funkcija za inicijalizaciju pina
 * @param GPIOx Port pina
 * @param GPIOPin Redni broj pina
 * @param GPIOMode Enumeracija moda pina (IN, OUT, ANALOG, AF)
 * @param GPIOOType Enumeracija tipa pina (OpenDrain ili PushPull)
 * @param GPIOPuPd Enumeracija PuPd (NOPULL, DOWN, UP)
 * @param GPIOSpeed Enumeracija brzine (low, mid, fast, high)
*/
void InitGPIO(GPIO_TypeDef* GPIOx, uint32_t GPIOPin, GPIOMode GPIOMode, GPIOOType GPIOOType, GPIOPuPd GPIOPuPd, GPIOSpeed GPIOSpeed){
	GPIO_InitTypeDef pin;			//		^ probaj enum ovde
	pin.GPIO_Mode = GPIOMode;
	pin.GPIO_OType = GPIOOType;
	pin.GPIO_Pin = GPIOPin;
	pin.GPIO_PuPd = GPIOPuPd;
	pin.GPIO_Speed = GPIOSpeed;
	GPIO_Init(GPIOx, &pin);
}
/**
 * GPIO funkcija za setovanje na odredjeni mod (IN, OUT, AF)
 * @param GPIOx Port pina
 * @param GPIOPin Redni broj pina
 */
void GPIOSetPinModIn(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	uint8_t i;
	for (i = 0x00; i < 0x10; i++) {
		if (GPIO_Pin & (1 << i)) {
			/* Set 00 kombinaciju za input*/
			GPIOx->MODER |= (0x03 << (2 * i));
		}
	}
}
void GPIOSetPinModeOut(GPIO_TypeDef* GPIOx, uint32_t GPIOPin){
	uint8_t i;
	for (i = 0x00; i < 0x10; i++) {
		if (GPIOPin & (1 << i)) {
			/* Set 01 kombinaciju za input*/
			GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x01 << (2 * i));
		}
	}
}
void GPIOSetPinModeAF(GPIO_TypeDef* GPIOx, uint32_t GPIOPin) {
	uint8_t i;
	for (i = 0x00; i < 0x10; i++) {

		if ((GPIOPin & (1 << i)) == 0) {
			continue;
		}
		/* 10 alternativni*/
		GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x02 << (2 * i));
	}
}

/**
 * Podesavanje pina na PullUp, PullDown ili NoPull
 * @param GPIOx Port pina
 * @param GPIOPin Redni broj pina
 *
 */
void GPIOSetPu(GPIO_TypeDef* GPIOx, uint32_t GPIOPin){
	uint8_t pinpos;
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		if ((GPIOPin & (1 << pinpos)) == 0)
			continue;
		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd_UP << (2 * pinpos)));
	}
}
void GPIOSetPd(GPIO_TypeDef* GPIOx, uint32_t GPIOPin){
	uint8_t pinpos;
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		if ((GPIOPin & (1 << pinpos)) == 0)
			continue;
		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd_DOWN << (2 * pinpos)));
	}
}
void GPIOSetNoPuPd(GPIO_TypeDef* GPIOx, uint32_t GPIOPin){
	uint8_t pinpos;
	for (pinpos = 0; pinpos < 0x10; pinpos++) {
		if ((GPIOPin & (1 << pinpos)) == 0)
			continue;
		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd_NOPULL << (2 * pinpos)));
	}
}


