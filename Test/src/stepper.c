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
 * Initializing GPIO for stepper motor
 */
void Step_Init(){
	GPIOInit(GPIOB, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	GPIOInit(GPIOB, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	GPIOInit(GPIOB, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	GPIOInit(GPIOB, GPIO_Pin_15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	Step_Stepback();
	Delay(250);
}
/**
 * Step until one ball can pass through
 */
void Step_Step(){
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		Step_Sequence_fwd();
	while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		Step_Sequence_fwd();
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		Step_Sequence_fwd();
	while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		Step_Sequence_bcwd();
}
/**
 * Step back until one ball can get out if stuck in feeder
 */
void Step_Stepback(){
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		Step_Sequence_bcwd();
	while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		Step_Sequence_bcwd();
}
/**
 * Forward sequence
 */
void Step_Sequence_fwd(){
	Delay(10);

	GPIO_SetBits(GPIOB, GPIO_Pin_1); //GPIO_Write();
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	Delay(10);

	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	Delay(10);

	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	Delay(10);

	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);

}
/**
 * Backward sequence
 */
void Step_Sequence_bcwd(){
	Delay(10);

	GPIO_SetBits(GPIOB, GPIO_Pin_1); //GPIO_Write();
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	Delay(10);

	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	Delay(10);

	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	Delay(10);

	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

