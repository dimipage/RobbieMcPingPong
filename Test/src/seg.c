/*
 * 7seg.c
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "seg.h"

uint8_t mode_flag = -1; //-1 - za spin, 1 - za field
volatile uint8_t cnt;
uint8_t num_array[10] = { 0xBF, 0x06, 0x5B, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF};
/**
 *
 */
void SEG_Init(){
/*clk*/		InitGPIO(GPIOC, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/*data*/	InitGPIO(GPIOC, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/* !EO*/	InitGPIO(GPIOA, GPIO_Pin_10, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/* rclk*/	InitGPIO(GPIOA, GPIO_Pin_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);

//timer

}
/**
 *
 * @param num
 */
void SEG_DisplayNumber(uint8_t num){
	int i;
	int j = 0;
	volatile uint8_t BR = num_array[num];
	if(num > -1 && num < 10){
		//GPIO_ResetBits(GPIOA, GPIO_Pin_10);
		for(i = 0; i<8; i++){
			GPIO_SetBits(GPIOC, GPIO_Pin_1);
			if(BR & 0x01 > 0x00){
				for(j = 10; j>0; j--){}
				GPIO_SetBits(GPIOC, GPIO_Pin_0);
				for(j = 10; j>0; j--){}
			}else{
				for(j = 10; j>0; j--){}
				GPIO_ResetBits(GPIOC, GPIO_Pin_0);
				for(j = 10; j>0; j--){}
			}
			GPIO_ResetBits(GPIOC, GPIO_Pin_1);
			BR >>= 1;
			for(j = 10; j>0; j--){}
		}
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		for(j = 10; j>0; j--){}
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);

		GPIO_SetBits(GPIOA, GPIO_Pin_10);
		for(j = 10; j>0; j--){}
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	}
}
/**
 *
 */
void SEG_Inc(){
	if(mode_flag)
		if(cnt == FIELDS)
			cnt = 0;
		else cnt++;
	else
		if(cnt == SPIN)
			cnt = 0;
		else cnt++;
	SEG_DisplayNumber(cnt);
}
