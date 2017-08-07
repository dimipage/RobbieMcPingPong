/*
 * stepper.c
 *
 *  Created on: Aug 4, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "stepper.h"
// TIM_TimeBaseInitTypeDef timer;

int step_counter = 0;
static void Step_Sequence_fwd();
static void Step_Sequence_bcwd();

/**
 *
 */
void Step_Init(){
	InitGPIO(GPIOB, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	InitGPIO(GPIOB, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	InitGPIO(GPIOB, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	InitGPIO(GPIOB, GPIO_Pin_15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
}
/**
 *
 */
void Step_Step(){
	int counter = 67;
	if(step_counter < 2)
		counter = 66;
	else
		step_counter = -1;
	for(; counter > 0; counter--)
		Step_Sequence_fwd();
	step_counter++;
}
/**
 *
 */
void Step_Stepback(){
	int counter = 66;
	if(step_counter == 2)
		counter = 67;
	else
		if(step_counter == 0)
			step_counter = 3;
	for(; counter > 0; counter--)
		Step_Sequence_bcwd();
	step_counter--;
}
/**
 *
 */
static void Step_Sequence_fwd(){
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
	//GPIO_ResetBits(GPIOB, GPIO_Pin_1); //????
	//Delay(5);
}
/**
 *
 */
static void Step_Sequence_bcwd(){
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
