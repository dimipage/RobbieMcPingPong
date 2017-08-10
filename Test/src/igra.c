/*
 * igra.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "igra.h"

GPIO_InitTypeDef btn_set, btn_mode, btn_start;
GPIO_InitTypeDef nucleo_led, led;
GPIO_InitTypeDef sen_top, sen_bottom;
SEG_Disp seg_disp;
Dequeue deq;

uint8_t start = 1;
uint8_t ball_counter;
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
	//led
	LED_Init();
	led = GPIOInit(GPIOA, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_100MHz);

	Btn_Init(POWER, ENABLE); //dugme power_off za state0
	SEG_Init(&seg_disp); 	 //7seg
	SysTickInit(1000); 		//systick cnt 1ms
	DC_Init();				//dc motori init
	RC_Init();				//servo
	Step_Init();			//stepper
	Dequeue_Init(&deq); 	//dek

	State1();
}
void State1(){
	LED_LightOff();
	Ball_typedef ball;
	extern State2_flag;

	//set i mode btns GPIO
	btn_set = GPIOInit(GPIOC, GPIO_Pin_10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	btn_mode = GPIOInit(GPIOC, GPIO_Pin_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//dugme start al nema trenutno interrupt
	btn_start = GPIOInit(GPIOC, GPIO_Pin_12, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_High_Speed);

	SEG_DisplayNumber(1, &seg_disp);

	SEG_ChangeMode(spin, &seg_disp);

	while(1){

		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)){//mode dugme
			SEG_Inc(&seg_disp);
			Delay(500);
		}
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)){//set dugme
			SEG_Off(&seg_disp);
			Delay(250);
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
		if(State2_flag > 0)
			State2();

	}
}
void State2(){ //start_pauza flag?
	LED_LightOn();
	Step_BackInit();
	if(start > 0){
		//kod za start
		start = -start;
		while(Dequeue_GetCounter(&deq) > 0){
			Ball_typedef ball;
			ball = pop(&deq);
			switch(ball.spin){
			case 1: //levi
				//DC_SetSpeed(MOT1, 100);//PC6
				DC_SetSpeed(MOT2, 100);//PC8
				//DC_SetSpeed(MOT3, 75);//PC9
				break;
			case 2: //desni
				//DC_SetSpeed(MOT1, 100);//PC6
				DC_SetSpeed(MOT2, 75);//PC8
				//DC_SetSpeed(MOT3, 100);//PC9
				break;
			case 3:	//gore
				//DC_SetSpeed(MOT1, 75);//PC6
				DC_SetSpeed(MOT2, 100);//PC8
				//DC_SetSpeed(MOT3, 100);//PC9
				break;
			case 4: //dole
				//DC_SetSpeed(MOT1, 75);//PC6
				DC_SetSpeed(MOT2, 75);//PC8
				//DC_SetSpeed(MOT3, 75);//PC9
				break;
			}
			Step_Step();
			Delay(1000);
			//servo pre ili posle, bolje posle za random mod
			//servo field
		}
	}else{
		//kod za pauzu
		start = -start;

	}
}
void State3(){ //init 0

}
