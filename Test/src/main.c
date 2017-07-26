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

int main(void)
{
	TIM_TimeBaseInitTypeDef timer_test;
	GPIO_InitTypeDef pin_led;
	NVIC_InitTypeDef nested_vector;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	timer_test.TIM_Prescaler = 0;
	timer_test.TIM_Period = 8399;
	timer_test.TIM_CounterMode = TIM_CounterMode_Up;
	timer_test.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_test.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timer_test);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	nested_vector.NVIC_IRQChannel = TIM4_IRQn;
	nested_vector.NVIC_IRQChannelPreemptionPriority = 0;
	nested_vector.NVIC_IRQChannelSubPriority = 1;
	nested_vector.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nested_vector);

	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM4);

	pin_led.GPIO_Pin = GPIO_Pin_5;
	pin_led.GPIO_OType = GPIO_OType_PP;
	pin_led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pin_led.GPIO_Mode = GPIO_Mode_OUT;
	pin_led.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &pin_led);

	for(;;){
/*		int timerValue = TIM_GetCounter(TIM4);
		if (timerValue > 400){
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}	******TIMER TEST******
*/

	}
}
void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
