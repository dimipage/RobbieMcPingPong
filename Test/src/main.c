/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


//#include "stm32f4xx.h"
#include "gpio.h"
#include "stm32f4xx_tim.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_gpio.h"


#define SERVO_MAX 5569
#define SERVO_MID 4914
#define SERVO_MIN 4259

int INCREMENT = 4;
TIM_OCInitTypeDef pwm; //trenutno

int main(void)
{
	TIM_TimeBaseInitTypeDef timer_test;
//	GPIO_InitTypeDef pin_led;
//	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef nested_vector;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


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
	65% duty cycle:     pulse_length = ((65535 + 1) * 6.5) / 100 - 1 = 4259
	75% duty cycle:     pulse_length = ((65535 + 1) * 7.5) / 100 - 1 = 4914                      +2?
	85% duty cycle:    	pulse_length = ((65535 + 1) * 8.5) / 100 - 1 = 5569
*/
	pwm.TIM_Pulse = 4914;
	TIM_OC1Init(TIM4, &pwm);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	nested_vector.NVIC_IRQChannel = TIM4_IRQn;
	nested_vector.NVIC_IRQChannelPreemptionPriority = 0;
	nested_vector.NVIC_IRQChannelSubPriority = 1;
	nested_vector.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nested_vector);

	InitGPIO(GPIOA, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	InitGPIO(GPIOB, GPIO_Pin_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

/*	pin_led.GPIO_Pin = GPIO_Pin_5;
	pin_led.GPIO_OType = GPIO_OType_PP;
	pin_led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pin_led.GPIO_Mode = GPIO_Mode_OUT;
	pin_led.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &pin_led);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
*/
/*	za stepper

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
*/
	for(;;){

	}
}
void TIM4_IRQHandler()
{

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		GPIO_ToggleBits(GPIOA, GPIO_PinSource5);
		if(pwm.TIM_Pulse > SERVO_MAX || pwm.TIM_Pulse < SERVO_MIN)
				INCREMENT = -INCREMENT;

		pwm.TIM_Pulse += INCREMENT;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TIM_OC1Init(TIM4, &pwm);

	}

}
