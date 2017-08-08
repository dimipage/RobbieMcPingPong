/*
 * button.c
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "button.h"

/**
 * Podesavanje namenskih dugmica za set, mode, start, poweroff
 * @param p Redni broj pina
 * @param EXTI_PinSrcX Izvor pina eksternog prekida
 * @param EXTIx_IRQn Prekidna linija
 * @param x Port pina
 * @param EXTI_PortSrcGPIOx Izvor porta
 * @param st ENABLE v DISABLE
 * @param tr Nacin okidanja
 * @param prior Prioritet prekida
 * @param subprior Podprioritet prekida
 */
void Btn_Init(uint16_t p, uint8_t EXTI_PinSrcX, uint8_t EXTIx_IRQn, GPIO_TypeDef* x, uint8_t EXTI_PortSrcGPIOx, FunctionalState st, EXTITrigger_TypeDef tr, uint8_t prior, uint8_t subprior){
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	GPIOInit(x, p, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_High_Speed);

	SYSCFG_EXTILineConfig(EXTI_PortSrcGPIOx, EXTI_PinSrcX);

	exti.EXTI_Line =(uint32_t) p; //ne radi za alternative_int (posle pin 15), svejedno za to nije namenjeno
	exti.EXTI_LineCmd = st;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = tr;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTIx_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = prior;
	nvic.NVIC_IRQChannelSubPriority = subprior;
	nvic.NVIC_IRQChannelCmd = st;
	NVIC_Init(&nvic);
}


