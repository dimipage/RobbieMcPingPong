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

#include "led.h"
#include "gpio.h"
#include "pwm.h"
#include "DCMotor.h"
#include "RCServo.h"

#define DEBOUNCE 30
//int INCREMENT = 4;
 //trenutno
volatile uint32_t btnticks = 0;
extern volatile uint32_t ticks = 0;
LED_InitType led, led2;

int main(void){

	EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

/*425686*/RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	led = LED_Init(SYSTEM_NON_INIT, GPIO_Pin_13, GPIOB);
	led2 = LED_Init(SYSTEM_OUT_OF_BALLS, GPIO_Pin_5, GPIOA);
	InitGPIO(GPIOC,GPIO_Pin_13, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13); //dozvola spoljasnjeg prekida na line13 portC

	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn; //kanal za line10~15
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	DC_Init(); //TIM3, PC6,8,9-->MOT1,2,3
	DC_SetSpeed(MOT1, SPEED_MID);

	RC_Init(); //TIM3, PB6-->jedan jedini

	SysTickInit(1000); //1ms
	for(;;){

	}
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
void SysTick_Handler(){
	ticks++;
	LED_Update(&led);
	LED_Update(&led2);

}
void SysTickInit (uint16_t frequency){
   RCC_ClocksTypeDef RCC_Clocks;
   RCC_GetClocksFreq (&RCC_Clocks);
   SysTick_Config (RCC_Clocks.HCLK_Frequency / frequency);
}
void EXTI15_10_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line13) != RESET){
		btnticks = 0;
		while(btnticks < DEBOUNCE){

		}
		//upali usr_led recimo
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}
