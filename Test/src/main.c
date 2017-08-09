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

#include "igra.h"
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

int main(void){
/*425686*/
	State0();

}
void EXTI2_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line2) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line2);
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

