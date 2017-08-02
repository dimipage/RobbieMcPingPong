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
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"


#include "led.h"
#include "gpio.h"
#include "pwm.h"
#include "DCMotor.h"
#include "RCServo.h"

#define DEBOUNCE 50
//int INCREMENT = 4;
 //trenutno
volatile uint32_t btn_start_time = 0;
volatile int btn_flag_set = -1; // -1:FALSE 1:TRUE
extern volatile uint32_t ticks = 0;

volatile int falling_edge = 1;
volatile int rising_edge = -1;
volatile int click = -1;
LED_InitType led, led2;

int main(void){

	EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

/*425686*/RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	led = LED_Init(SYSTEM_NON_INIT, GPIO_Pin_13, GPIOB);
	//led2 = LED_Init(SYSTEM_OUT_OF_BALLS, GPIO_Pin_5, GPIOA);
	InitGPIO(GPIOC, GPIO_Pin_13, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
	InitGPIO(GPIOA, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	SysTickInit(1000); //1ms

	EXTI_ClearITPendingBit(EXTI_Line13);//1
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	EXTI_ClearITPendingBit(EXTI_Line13);//2

	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn; //kanal za liniju13
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	EXTI_ClearITPendingBit(EXTI_Line13);//3


	//DC_Init(); //TIM3, PC6,8,9-->MOT1,2,3
	//DC_SetSpeed(MOT1, SPEED_MID);

	//RC_Init(); //TIM3, PB6-->jedan jedini

	for(;;){

	}
}
void EXTI15_10_IRQHandler(){

	if (EXTI_GetITStatus(EXTI_Line13) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line13);
		if(rising_edge > 0){
			falling_edge = -falling_edge;
			if(ticks - btn_start_time > DEBOUNCE)
				//btn_flag_set = -btn_flag_set;
				click = -click;

			if(click > 0)
				GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}else if(falling_edge > 0){
			rising_edge = -rising_edge;
			btn_start_time = ticks;
			//btn_flag_set = -btn_flag_set;
		}
	}
}
void SysTick_Handler(){
	ticks++;
	//LED_Update(&led); //kroz main
	//LED_Update(&led2);
}
void SysTickInit (uint16_t frequency){
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);
	SysTick_Config (RCC_Clocks.HCLK_Frequency / frequency);
	NVIC_SetPriority(SysTick_IRQn, 0); //priority veci od EXTI
}
void TIM4_IRQHandler(){
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		RC_SetAngle(0.0);
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		DC_SetSpeed(MOT1, SPEED_MID);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
