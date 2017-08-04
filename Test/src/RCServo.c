/*
 * RCServo.c
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "RCServo.h"
#include "pwm.h"
#include "gpio.h"

#define INIT_DUTY_CYCLE 7.5
#define INIT_ANGLE_PULSE 4914
#define ANGLE_STEP 18.2

static TIM_TimeBaseInitTypeDef timer_test;
static TIM_OCInitTypeDef pwm;

/**
 *
 */
void RC_Init(){
	NVIC_InitTypeDef nested_vector;

	timer_test.TIM_Prescaler = 30;
	timer_test.TIM_Period = 65535;
	timer_test.TIM_CounterMode = TIM_CounterMode_Up;
	timer_test.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_test.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timer_test);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	pwm.TIM_OCMode = TIM_OCMode_PWM1; 					//PWM1 = Set on compare match ====== PWM2 = Clear on compare match
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
/*
	pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
	6.5% duty cycle:     pulse_length = ((65535 + 1) * 6.5) / 100 - 1 = 4259
	7.5% duty cycle:     pulse_length = ((65535 + 1) * 7.5) / 100 - 1 = 4914                      +2?
	8.5% duty cycle:  	 pulse_length = ((65535 + 1) * 8.5) / 100 - 1 = 5569
	6552
*/
	RC_SetAngle(0.0);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	nested_vector.NVIC_IRQChannel = TIM4_IRQn;
	nested_vector.NVIC_IRQChannelPreemptionPriority = 1;
	nested_vector.NVIC_IRQChannelSubPriority = 0;
	nested_vector.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nested_vector);

	InitGPIO(GPIOB, GPIO_Pin_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

	TIM_OC1Init(TIM4, &pwm); //mozda
}
/**
 *
 */
void RC_SetAngle(double angle){
	if(angle > -90.0 && angle < 90.0)
		PWM_SetPulse(&pwm,(uint32_t) (INIT_ANGLE_PULSE + (ANGLE_STEP * angle)));
	TIM_OC1Init(TIM4, &pwm);
}
/**
 *
 * @param pwmOC
 * @return
 */
double RC_GetAngle(){
	return PWM_GetPulse(&pwm) - INIT_ANGLE_PULSE / ANGLE_STEP;
}

