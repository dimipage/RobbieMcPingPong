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
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	pwm.TIM_OCMode = TIM_OCMode_PWM1; //PWM1 = Set on compare match | PWM2 = Clear on compare match
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	pwm.TIM_Pulse = 0; //inicijalno

	TIM_OC1Init(TIM3, &pwm);
	TIM_OC3Init(TIM3, &pwm);
	TIM_OC4Init(TIM3, &pwm);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	nested_vector.NVIC_IRQChannel = TIM3_IRQn;
	nested_vector.NVIC_IRQChannelPreemptionPriority = 1;
	nested_vector.NVIC_IRQChannelSubPriority = 0;
	nested_vector.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nested_vector);

	GPIOInit(GPIOC, GPIO_Pin_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	GPIOInit(GPIOC, GPIO_Pin_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	GPIOInit(GPIOC, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
}
/**
 * Podesavanje brzine DC motora
 * @param motor Motor koji se podesava
 * @param speed Brzina okretanja motora
 */
void DC_SetSpeed(DC_Motor motor, DC_Speed speed){
	PWM_SetDutyCycle(&pwm, speed, &timer);
	switch (motor){
	case MOT1:
		TIM_OC1Init(TIM3, &pwm);
		break;
	case MOT2:
		TIM_OC3Init(TIM3, &pwm);
		break;
	case MOT3:
		TIM_OC4Init(TIM3, &pwm);
		break;
	}
}
