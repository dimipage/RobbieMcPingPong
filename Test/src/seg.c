/*
 * 7seg.c
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "seg.h"

#define SPIN 4 ///imamo 4 moguca spina
#define FIELDS 6 ///6 polja za gadjanje na stolu

void SEG_Init(SEG_Disp* disp){
/*sclk*/	GPIOInit(GPIOC, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/*data*/	GPIOInit(GPIOC, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/* !EO*/	GPIOInit(GPIOB, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/* rclk*/	GPIOInit(GPIOA, GPIO_Pin_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
	disp->cnt = 1;
	disp->num_array[0] = 0xBF; disp->num_array[1] = 0x06; disp->num_array[2] = 0x5B; disp->num_array[3] = 0xCF;
	disp->num_array[4] = 0xE6; disp->num_array[5] = 0xED; disp->num_array[6] = 0xFD; disp->num_array[7] = 0x07;
	disp->num_array[8] = 0xFF; disp->num_array[9] = 0xEF;
}


void SEG_DisplayNumber(uint8_t num, SEG_Disp* disp){
	int segment_counter;
	volatile uint8_t BR = disp->num_array[num];
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
		disp->cnt = num;
	}
}

void SEG_Inc(SEG_Disp* disp){
	switch(disp->mode){
	case spin:
		if(disp->cnt == SPIN)
			disp->cnt = 1;
		else disp->cnt++;
		break;
	case field:
		if(disp->cnt == FIELDS)
			disp->cnt = 1;
		else disp->cnt++;
		break;
	}
	SEG_DisplayNumber(disp->cnt, disp);
}

void SEG_ChangeMode(MODE_FLAG m, SEG_Disp* disp){
	disp->mode = m;
}

int SEG_GetNumber(SEG_Disp* disp){
	return disp->cnt;
}

MODE_FLAG SEG_GetMode(SEG_Disp* disp){
	return disp->mode;
}
