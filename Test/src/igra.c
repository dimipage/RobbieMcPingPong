/*
 * igra.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "igra.h"

GPIO_InitTypeDef btn_set, btn_mode;
GPIO_InitTypeDef nucleo_led;
GPIO_InitTypeDef sen_top, sen_bottom;
SEG_Disp seg_disp;
Dequeue deq;
BTN_EXTI btn_start;
void State0(){ //init 1
	//SYSTick clk enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//Port clk enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 	//Timers clk enable
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

 	//senzori GPIO
	sen_top = GPIOInit(GPIOB, GPIO_Pin_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	sen_bottom = GPIOInit(GPIOB, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//set i mode btns GPIO
	btn_set = GPIOInit(GPIOC, GPIO_Pin_10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	btn_mode = GPIOInit(GPIOC, GPIO_Pin_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//led
	LED_Init();
	GPIOInit(GPIOA, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);
	//dugme power_off za state0
	Btn_Init(POWER, ENABLE);
	//7seg
	SEG_Init(&seg_disp);
	//systick cnt
	SysTickInit(1000); //1ms
	//dc motori
	DC_Init();
	//servo
	RC_Init();
	//stepper
	Step_Init();
	Dequeue_Init(&deq);
	State1();
}
void State1(){
	Ball_typedef ball;
	//GPIO za dugmice SET i MODE

	//dugme start za interrupt
	Btn_Init(START, ENABLE);
	SEG_DisplayNumber(1, &seg_disp);
	SEG_ChangeMode(spin, &seg_disp);

	while(1){

		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)){//mode dugme
			SEG_Inc(&seg_disp);
			Delay(500);
		}
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)){//set dugme
			if(SEG_GetMode(&seg_disp) == spin){ // if fels
				ball.spin = SEG_GetNumber(&seg_disp);
				SEG_DisplayNumber(1, &seg_disp);
				SEG_ChangeMode(field, &seg_disp);
			}
			else if(SEG_GetMode(&seg_disp) == field){// if polje
				ball.field = SEG_GetNumber(&seg_disp);
				SEG_DisplayNumber(1, &seg_disp);
				SEG_ChangeMode(spin, &seg_disp);
				if(push(ball, &deq) == 0)
					LED_LightOn();
			}
			Delay(500);
		}
	}
}
void State2(){ //start_pauza flag?

}
void State3(){ //init 0

}
