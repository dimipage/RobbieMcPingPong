/*
 * stepper.c
 *
 *  Created on: Aug 4, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "stepper.h"
TIM_TimeBaseInitTypeDef timer;

/**
 *
 */
void Step_Init(){
	InitGPIO(GPIOB, GPIO_Pin_7, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	InitGPIO(GPIOC, GPIO_Pin_13, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	InitGPIO(GPIOC, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);
	InitGPIO(GPIOC, GPIO_Pin_15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_100MHz);

	timer.TIM_Prescaler = 30;
	timer.TIM_Period = 65535;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timer);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

}
/**
 *
 */
void Step_Step(){


}
/**
 *
 */
void Step_Stepback(){

}
/**
 *
 */
void Step_Sequence(){
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	Delay(5);
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	Delay(5);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
	Delay(5);
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	Delay(5);
}
