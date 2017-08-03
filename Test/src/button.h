/*
 * button.h
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"

#include "gpio.h"

typedef struct{ //nije u upotrebi
	uint16_t pin;
	GPIO_TypeDef* GPIOx;
	FunctionalState state;
	EXTITrigger_TypeDef trigger;
	uint8_t priority;
	uint8_t subpriority;
}button;

void Btn_Init(uint16_t p, uint8_t EXTI_PinSrcX, uint8_t EXTIx_IRQn, GPIO_TypeDef* x, uint8_t EXTI_PortSrcGPIOx, FunctionalState st, EXTITrigger_TypeDef tr, uint8_t prior, uint8_t subprior);
#endif /* BUTTON_H_ */
