/*
 * stepper.c
 *
 *  Created on: Aug 4, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "stepper.h"
// TIM_TimeBaseInitTypeDef timer;

int step_counter = 0;

/**
 * Inicijalizacija GPIO za stepper
 */
void Step_Init(){
	GPIOInit(GPIOB, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	GPIOInit(GPIOB, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	GPIOInit(GPIOB, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	GPIOInit(GPIOB, GPIO_Pin_15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
}
/**
 * Stepper pravi jedan korak tj pomeraj da kuglica prodje do DC motora
 */
void Step_Step(){
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)){
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_fwd();
		while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_fwd();
	}
	else while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_fwd();
}
/**
 * Stepper pravi jedan korak tj pomeraj unazad
 */
void Step_Stepback(){
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)){
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_bcwd();
		while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_bcwd();
	}
	else while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_bcwd();
}
/**
 * Sekvenca za pomeranje stepera u smeru kazaljke na satu
 * realizovana preko GPIO
 */
void Step_Sequence_fwd(){
	Delay(15);

	GPIO_SetBits(GPIOB, GPIO_Pin_1); //GPIO_Write();
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	Delay(15);

	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	Delay(15);

	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	Delay(15);

	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);

}
/**
 * Sekvenca za pomeranje stepera u smeru suprotnom kazaljke na satu
 * realizovana preko GPIO
 */
void Step_Sequence_bcwd(){
	Delay(15);

	GPIO_SetBits(GPIOB, GPIO_Pin_1); //GPIO_Write();
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	Delay(15);

	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	Delay(15);

	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	Delay(15);

	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}



void Step_BackInit(){
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)){
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_bcwd();
		while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_bcwd();
	}
	else while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
			Step_Sequence_bcwd();
}
