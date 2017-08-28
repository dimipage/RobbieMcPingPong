/*
 * DCMotor.c
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "DCMotor.h"


TIM_OCInitTypeDef pwm;
TIM_TimeBaseInitTypeDef timer;

void DC_SetSpeed(DC_Motor motor, DC_Speed speed){

	TIM_OCStructInit(&pwm);
	pwm.TIM_OCMode = TIM_OCMode_PWM1; //PWM1 = Set on compare match | PWM2 = Clear on compare match
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	pwm.TIM_Pulse = (5080 / 100) * speed;

	switch(motor){
	case MOT1:
		GPIOInit(GPIOC, GPIO_Pin_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);
		Delay(10);
		TIM_OC1Init(TIM3, &pwm);
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_CtrlPWMOutputs(TIM3, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		GPIOInit(GPIOC, GPIO_Pin_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
		break;
	case MOT2:
		GPIOInit(GPIOC, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);
		Delay(10);
		TIM_OC3Init(TIM3, &pwm);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_CtrlPWMOutputs(TIM3, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		GPIOInit(GPIOC, GPIO_Pin_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
		break;
	case MOT3:
		GPIOInit(GPIOC, GPIO_Pin_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);
		Delay(10);
		TIM_OC4Init(TIM3, &pwm);
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_CtrlPWMOutputs(TIM3, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		GPIOInit(GPIOC, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_25MHz);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
		break;
	}
}
/**
 * Inicijalizacija TIM3 i PWM na PC6,8,9 za DC motore
 */
void DC_Init(){
	NVIC_InitTypeDef nested_vector;

	timer.TIM_Prescaler = 0; //20KHz
	timer.TIM_Period = 5080;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timer);
//	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_OCStructInit(&pwm);
	pwm.TIM_OCMode = TIM_OCMode_PWM1; //PWM1 = Set on compare match | PWM2 = Clear on compare match
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	pwm.TIM_Pulse = 5000; //inicijalno

	TIM_OC1Init(TIM3, &pwm);
	TIM_OC3Init(TIM3, &pwm);
	TIM_OC4Init(TIM3, &pwm);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//NOVO
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	//POMERENO
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

void DC_SetTopSpeed(Ball_typedef* ball){
	extern ticks;

	switch(ball->spin){
	case 0:
		ball->spin = (ticks % 4) + 1;
		DC_SetTopSpeed(ball);
		break;
	case 1:	//normalni
		DC_SetSpeed(MOT2, 15);//PC6 60
		DC_SetSpeed(MOT1, 62);//PC8 60
		DC_SetSpeed(MOT3, 62);//PC9 55
		break;
	case 2:	//levi
		DC_SetSpeed(MOT2, 75);//PC6 75
		DC_SetSpeed(MOT1, 25);//PC8 36
		DC_SetSpeed(MOT3, 75);//PC9 67
		break;
	case 3:	//desni
		DC_SetSpeed(MOT2, 75);//PC6 75
		DC_SetSpeed(MOT1, 75);//PC8 68
		DC_SetSpeed(MOT3, 25);//PC9 35
		break;
	case 4:	//gornji
		DC_SetSpeed(MOT2, 15);//PC6 15
		DC_SetSpeed(MOT1, 62);//PC8 75
		DC_SetSpeed(MOT3, 62);//PC9 75
		break;
	}
}
void DC_SetBotSpeed(Ball_typedef* ball){
	extern ticks;
	switch(ball->spin){
	case 0:
		ball->spin = (ticks % 4) + 1;
		DC_SetBotSpeed(ball);
		break;
	case 1:	//normalni
		DC_SetSpeed(MOT2, 77);//PC6 63
		DC_SetSpeed(MOT1, 62);//PC8 62
		DC_SetSpeed(MOT3, 62);//PC9 62
		break;
	case 2:	//levi
		DC_SetSpeed(MOT2, 72);//PC6 77
		DC_SetSpeed(MOT1, 72);//PC8 44
		DC_SetSpeed(MOT3, 47);//PC9 69
		break;
	case 3:	//desni
		DC_SetSpeed(MOT2, 72);//PC6 79
		DC_SetSpeed(MOT1, 47);//PC8 70 moze i 80
		DC_SetSpeed(MOT3, 72);//PC9 41
		break;
	case 4:	//gornji
		DC_SetSpeed(MOT2, 35);//PC6 35
		DC_SetSpeed(MOT1, 65);//PC8 83
		DC_SetSpeed(MOT3, 65);//PC9 83
		break;
	}
}
