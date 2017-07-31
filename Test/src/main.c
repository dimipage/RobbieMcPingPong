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

#include "gpio.h"
#include "pwm.h"
#include "DCMotor.h"
#include "RCServo.h"

//int INCREMENT = 4;
 //trenutno

int main(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	DC_Init(); //TIM3, PC6,8,9-->MOT1,2,3
	DC_SetSpeed(MOT1, SPEED_MID);

	RC_Init(); //TIM3, PB6-->jedan jedini


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
