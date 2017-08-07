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


#include "stepper.h"
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
/*425686*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//servo buttons
	InitGPIO(GPIOC, GPIO_Pin_10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	InitGPIO(GPIOC, GPIO_Pin_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//stepper buttons
	InitGPIO(GPIOC, GPIO_Pin_12, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	InitGPIO(GPIOD, GPIO_Pin_2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//
	led = LED_Init(SYSTEM_TEST, GPIO_Pin_13, GPIOB);
	InitGPIO(GPIOA, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);

	Btn_Init(GPIO_Pin_13, EXTI_PinSource13, EXTI15_10_IRQn, GPIOC, EXTI_PortSourceGPIOC ,ENABLE, EXTI_Trigger_Rising_Falling, 1, 1);

	SEG_Init();

	SysTickInit(1000); //1ms

	DC_Init();
	DC_SetSpeed(MOT1, SPEED_MEDIUM);
	DC_SetSpeed(MOT2, SPEED_HARD);
	DC_SetSpeed(MOT3, SPEED_BAJCHA);

	RC_Init();
	SEG_DisplayNumber(5);

	Step_Init();
	double angle = 0.0;

#define INCREMENT 5

	for(;;){/*
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)){
			angle -= INCREMENT;
			RC_SetAngle(angle);
			Delay(100);
		}
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)){
			angle += INCREMENT;
			RC_SetAngle(angle);
			Delay(100);
		}
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)){
			Step_Step();
			Delay(100);
		}
		if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)){
			Step_Stepback();
			Delay(100);
		}*/
		Step_Stepback();

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
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
void Delay(uint32_t t){
	  uint32_t start, end;
	  start = ticks;
	  end = start + t;
	  if (start < end)
	    while ((ticks >= start) && (ticks < end)){}
	  else
	    while ((ticks >= start) || (ticks < end)){};
}
