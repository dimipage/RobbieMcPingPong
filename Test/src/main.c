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
#define DEBOUNCE 40
volatile uint8_t exti_flag = 0;
volatile int exti_cnt = 0;
volatile uint8_t exti_reset = 0;

GPIO_InitTypeDef test_btn_rc, test_btn_rc2, exti_test;
GPIO_InitTypeDef test_btn_step, test_btn_step2;
GPIO_InitTypeDef nucleo_led;
GPIO_InitTypeDef sen_top, sen_bottom;
SEG_Disp seg;
extern uint8_t State2_flag = 0;


int main(void){
/*425686*/
	State0();

/*	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//senzori GPIO
	sen_top = GPIOInit(GPIOB, GPIO_Pin_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	sen_bottom = GPIOInit(GPIOB, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//servo buttons
	test_btn_rc = GPIOInit(GPIOC, GPIO_Pin_10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	test_btn_rc2 = GPIOInit(GPIOC, GPIO_Pin_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//stepper buttons
	test_btn_step = GPIOInit(GPIOC, GPIO_Pin_12, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	test_btn_step2 = GPIOInit(GPIOD, GPIO_Pin_2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//led
	LED_Init();
	nucleo_led = GPIOInit(GPIOA, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
	//dugmiki
	Btn_Init(POWER, ENABLE);
	exti_test = GPIOInit(GPIOC, GPIO_Pin_13, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_High_Speed);

	SEG_Init(&seg);

	SysTickInit(1000); //1ms

	DC_Init();
	//DC_SetSpeed(MOT1, 45);//PC6
	DC_SetSpeed(MOT2, 20);//PC8
	//DC_SetSpeed(MOT3, 65);//PC9

	RC_Init();
	SEG_DisplayNumber(3, &seg);

	Step_BackInit();

	for(;;){
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)){
			Step_Step();
			Delay(100);
		}
		if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)){
			Step_Stepback();
			Delay(100);
		}
	}*/

}
void EXTI2_IRQHandler(){

}
void EXTI15_10_IRQHandler(){

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

void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)){
			(exti_cnt > 0) ? (exti_cnt-=1) : (exti_cnt = 0);
			if(exti_flag > 0){
				if(exti_cnt < 59998 && exti_cnt > 800)//PRESS
					State2_flag = 1;
					//GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
				else if(exti_cnt >= 59999)		//RESET
					State1();//LED_LightOn();
				exti_flag = 0;
			}
		}else{
			(exti_cnt < 60000) ? (exti_cnt += 1) : (exti_cnt = 60000);
			exti_flag = 1;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
void TIM4_IRQHandler(){
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void Delay(uint32_t t){
	  uint32_t start, end;
	  start = ticks;
	  end = start + t;
	  if (start < end)
	    while ((ticks >= start) && (ticks < end)) { }
	  else
	    while ((ticks >= start) || (ticks < end)) { };
}

