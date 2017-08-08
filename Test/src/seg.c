/*
 * 7seg.c
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "seg.h"

#define SPIN 4 ///imamo 4 moguca spina
#define FIELDS 6 ///6 polja za gadjanje na stolu


MODE_FLAG mode;
volatile uint8_t cnt;
uint8_t num_array[10] = { 0xBF, 0x06, 0x5B, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF}; ///cifre za 7seg

/**
 * Inicijalizacija pinova za 7seg
 */
void SEG_Init(){
/*sclk*/	GPIOInit(GPIOC, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/*data*/	GPIOInit(GPIOC, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/* !EO*/	GPIOInit(GPIOB, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/* rclk*/	GPIOInit(GPIOA, GPIO_Pin_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
}

/**
 * Postavljanje cifre za 7seg
 * @param num Cifra koja se prikazuje
 */
void SEG_DisplayNumber(uint8_t num){
	int segment_counter;
	volatile uint8_t BR = num_array[num];
	if(num > -1 && num < 10){
		for(segment_counter = 0; segment_counter<=8; segment_counter++){
			GPIO_SetBits(GPIOC, GPIO_Pin_1);
			if((BR & 0x80) > 0x00){
				GPIO_SetBits(GPIOC, GPIO_Pin_0);
			}else{
				GPIO_ResetBits(GPIOC, GPIO_Pin_0);
			}
			GPIO_ResetBits(GPIOC, GPIO_Pin_1);
			BR <<= 1;
		}
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);

		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	}
}
/**
 * Inkrementiranje cifre na 7seg
 */
void SEG_Inc(){
	switch(mode){
	case spin:
		if(cnt == FIELDS)
			cnt = 0;
		else cnt++;
		break;
	case field:
		if(cnt == SPIN)
			cnt = 0;
		else cnt++;
		break;
	}
	SEG_DisplayNumber(cnt);
}
/**
 * Promena moda podesavanja robota
 * @param m Enumeracija za podesavanje spina ili polja
 */
void SEG_ChangeMode(MODE_FLAG m){
	mode = m;
}
