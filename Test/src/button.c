/*
 * button.c
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "button.h"

/**
 * Podesavanje GPIO, NVCI i EXTI za start ili power
 * @param btn Tip dugmeta sa prekidom; START ili POWER
 * @param st Stanje dugmeta; ENABLE ili DISABLE
 */
void Btn_Init(BTN_EXTI btn, FunctionalState st){
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	switch(btn){
	case START:
		GPIOInit(GPIOC, GPIO_Pin_12, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_High_Speed);
		exti.EXTI_Line = EXTI_Line12; //ne radi za alternative_int (posle pin 15), svejedno za to nije namenjeno

		nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 1;
		nvic.NVIC_IRQChannelSubPriority = 1;
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource12);
		break;
	case POWER:
		GPIOInit(GPIOA, GPIO_Pin_0, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_High_Speed);
		exti.EXTI_Line = EXTI_Line0; //ne radi za alternative_int (posle pin 15), svejedno za to nije namenjeno

		nvic.NVIC_IRQChannel = EXTI0_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 0;
		nvic.NVIC_IRQChannelSubPriority = 0;
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
		break;
	}

	exti.EXTI_LineCmd = st;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannelCmd = st;
	NVIC_Init(&nvic);
}


