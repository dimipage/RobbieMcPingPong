/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define SERVO_MAX 2000
#define SERVO_MIN 1000
#define SERVO_MID 1500
int counter = 0;
int threshold = 50;

int main(void)
{
	TIM_TimeBaseInitTypeDef timer_test;
	GPIO_InitTypeDef pin_led;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef nested_vector;
	TIM_OCInitTypeDef pwm;


 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	timer_test.TIM_Prescaler = 0;
	timer_test.TIM_Period = 18;
	timer_test.TIM_CounterMode = TIM_CounterMode_Up;
	timer_test.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_test.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timer_test);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	/* Common settings */

	/* PWM mode 2 = Clear on compare match */
	/* PWM mode 1 = Set on compare match */
	pwm.TIM_OCMode = TIM_OCMode_PWM1; 								//??
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;						//??

/*
	To get proper duty cycle, you have simple equation

	pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1

	where DutyCycle is in percent, between 0 and 100%

	25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
	50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
	75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
	100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399

	Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
*/
	pwm.TIM_Pulse = 50;
	TIM_OC1Init(TIM4, &pwm);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
/*
	pwm.TIM_Pulse = 100;
	TIM_OC2Init(TIM4, &pwm);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	pwm.TIM_Pulse = 150;
	TIM_OC3Init(TIM4, &pwm);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	pwm.TIM_Pulse = 200;
	TIM_OC4Init(TIM4, &pwm);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
*/
	nested_vector.NVIC_IRQChannel = TIM4_IRQn;
	nested_vector.NVIC_IRQChannelPreemptionPriority = 0;
	nested_vector.NVIC_IRQChannelSubPriority = 1;
	nested_vector.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nested_vector);



	pin_led.GPIO_Pin = GPIO_Pin_5;
	pin_led.GPIO_OType = GPIO_OType_PP;
	pin_led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pin_led.GPIO_Mode = GPIO_Mode_OUT;
	pin_led.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &pin_led);

	pin_led.GPIO_Pin = GPIO_Pin_13;
	pin_led.GPIO_OType = GPIO_OType_PP;
	pin_led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pin_led.GPIO_Mode = GPIO_Mode_AF;
	pin_led.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &pin_led);




    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; //| GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //?
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

  //  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

	for(;;){

	}
}
void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		if(counter < 100)
			counter++;
		else
			counter = 0;

		if(counter > threshold)
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		else
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
