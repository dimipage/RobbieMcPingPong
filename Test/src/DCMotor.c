/*
 * DCMotor.c
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "DCMotor.h"


TIM_OCInitTypeDef pwm;
TIM_TimeBaseInitTypeDef timer;

/**
 * Sets the speed of single DC
 * @param motor MOT1, MOT2 or MOT3 enum
 * @param speed Speed in percentage, lower % >> bigger speed
 */
void DC_SetSpeed(DC_Motor motor, uint8_t speed){

	TIM_OCStructInit(&pwm);
	pwm.TIM_OCMode = TIM_OCMode_PWM1; 								//PWM1 = Set on compare match | PWM2 = Clear on compare match
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	pwm.TIM_Pulse = (5080 / 100) * speed;

	switch(motor){
	case MOT1:
		GPIOInit(GPIOC, GPIO_Pin_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
		Delay(35);
		TIM_OC1Init(TIM3, &pwm);
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_CtrlPWMOutputs(TIM3, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		GPIOInit(GPIOC, GPIO_Pin_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
		break;
	case MOT2:
		GPIOInit(GPIOC, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
		Delay(35);
		TIM_OC3Init(TIM3, &pwm);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_CtrlPWMOutputs(TIM3, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		GPIOInit(GPIOC, GPIO_Pin_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
		break;
	case MOT3:
		GPIOInit(GPIOC, GPIO_Pin_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
		Delay(35);
		TIM_OC4Init(TIM3, &pwm);
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_CtrlPWMOutputs(TIM3, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		GPIOInit(GPIOC, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
		break;
	}
}
/**
 * Init TIM3 & PWM on PC6,8,9 for DC motors
 */
void DC_Init(){
	NVIC_InitTypeDef nested_vector;

	timer.TIM_Prescaler = 0; //20KHz
	timer.TIM_Period = 5080;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timer);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_OCStructInit(&pwm);
	pwm.TIM_OCMode = TIM_OCMode_PWM1; //PWM1 = Set on compare match | PWM2 = Clear on compare match
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	pwm.TIM_Pulse = 5079; 										//enough to stop

	TIM_OC1Init(TIM3, &pwm);
	TIM_OC3Init(TIM3, &pwm);
	TIM_OC4Init(TIM3, &pwm);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM3, ENABLE);

	TIM_Cmd(TIM3, ENABLE);

	nested_vector.NVIC_IRQChannel = TIM3_IRQn;
	nested_vector.NVIC_IRQChannelPreemptionPriority = 0;
	nested_vector.NVIC_IRQChannelSubPriority = 0;
	nested_vector.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nested_vector);

	GPIOInit(GPIOC, GPIO_Pin_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);
	GPIOInit(GPIOC, GPIO_Pin_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);
	GPIOInit(GPIOC, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
}
/**
 * Speed setting for upper fields
 * @param ball Ball struct
 */
void DC_SetTopSpeed(Ball_typedef* ball){
	extern ticks;

	switch(ball->spin){
	case 0:															//random spin
		ball->spin = (ticks % 4) + 1;
		DC_SetTopSpeed(ball);
		break;
	case 1://normal spin
		DC_SetSpeed(MOT2, 72);
		DC_SetSpeed(MOT3, 57);
		DC_SetSpeed(MOT1, 57);
		break;
	case 2:	//left
		DC_SetSpeed(MOT2, 70);
		DC_SetSpeed(MOT3, 70);
		DC_SetSpeed(MOT1, 40);
		break;
	case 3:	//right
		DC_SetSpeed(MOT2, 70);
		DC_SetSpeed(MOT3, 50);
		DC_SetSpeed(MOT1, 70);
		break;
	case 4:	//upper
		DC_SetSpeed(MOT2, 15);
		DC_SetSpeed(MOT3, 62);
		DC_SetSpeed(MOT1, 62);
		break;
	}
 }
/**
 * Set the speed of bottom fields
 * @param ball Ball struct
 */
void DC_SetBotSpeed(Ball_typedef* ball){
	extern ticks;
	switch(ball->spin){
	case 0:
		ball->spin = (ticks % 4) + 1;
		DC_SetBotSpeed(ball);
		break;
	case 1:	//normal spin
		DC_SetSpeed(MOT2, 77);
		DC_SetSpeed(MOT3, 62);
		DC_SetSpeed(MOT1, 62);

		break;
	case 2:	//left
		DC_SetSpeed(MOT2, 78);
		DC_SetSpeed(MOT3, 78);
		DC_SetSpeed(MOT1, 40);
		break;
	case 3:	//right
		DC_SetSpeed(MOT2, 72);
		DC_SetSpeed(MOT3, 55);
		DC_SetSpeed(MOT1, 72);

		break;
	case 4:	//upper
		DC_SetSpeed(MOT2, 35);
		DC_SetSpeed(MOT3, 65);
		DC_SetSpeed(MOT1, 65);
		break;
	}
}
