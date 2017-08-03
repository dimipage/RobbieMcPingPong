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

#include "button.h"
#include "led.h"
#include "gpio.h"
#include "pwm.h"
#include "DCMotor.h"
#include "RCServo.h"
#include "seg.h"

volatile uint32_t btn_start_time = 0;
volatile int btn_flag_set = -1; // -1:FALSE 1:TRUE
extern volatile uint32_t ticks = 0;
//debounce
#define DEBOUNCE 50
volatile int falling_edge = 1;
volatile int rising_edge = -1;
volatile int click = -1;
//
LED_InitType led, led2;

int main(void){

	EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

/*425686*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	led = LED_Init(SYSTEM_NON_INIT, GPIO_Pin_13, GPIOB);
	InitGPIO(GPIOA, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
	Btn_Init(GPIO_Pin_13, EXTI_PinSource13, EXTI15_10_IRQn, GPIOC, EXTI_PortSourceGPIOC ,ENABLE, EXTI_Trigger_Rising_Falling, 1, 1);
	SEG_Init();

	SysTickInit(1000); //1ms

	DC_Init(); //TIM3, PC6,8,9-->MOT1,2,3
	DC_SetSpeed(MOT1, SPEED_HIGH);
	DC_SetSpeed(MOT2, SPEED_MID);
	DC_SetSpeed(MOT3, SPEED_LOW);

	RC_Init(); //TIM3, PB6-->jedan jedini

	SEG_DisplayNumber(1);
	for(;;){

	}
}
void EXTI15_10_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line13) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line13);
		if(rising_edge > 0){
			falling_edge = -falling_edge;
			if(ticks - btn_start_time > DEBOUNCE)
				click = -click;
			if(click > 0)
				GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}else if(falling_edge > 0){
			rising_edge = -rising_edge;
			btn_start_time = ticks;
		}
	}
}
void SysTick_Handler(){
	ticks++;
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
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
