/*
 * 7seg.c
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "seg.h"

#define SPIN 4 ///spin number excluding random (0)
#define FIELDS 6 ///fields number excluding random (0)

/**
 * Init 7seg GPIO pins
 * @param disp Display struct
 */
void SEG_Init(SEG_Disp* disp){///made for multiple 7segs but realised with one
/* !OE*/	GPIOInit(GPIOC, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/*data*/	GPIOInit(GPIOC, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/* rclk*/	GPIOInit(GPIOB, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
/*srclk*/	GPIOInit(GPIOA, GPIO_Pin_4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_2MHz);
	disp->cnt = 0;																//init count is 0
	disp->num_array[0] = 0xBF; disp->num_array[1] = 0x06; disp->num_array[2] = 0x5B; disp->num_array[3] = 0xCF; //number bytes array
	disp->num_array[4] = 0xE6; disp->num_array[5] = 0xED; disp->num_array[6] = 0xFD; disp->num_array[7] = 0x07;
	disp->num_array[8] = 0xFF; disp->num_array[9] = 0xEF;
}
/**
 * Turn off display
 * @param disp Display struct
 */
void SEG_Off(SEG_Disp* disp){
	int segment_counter;
	volatile uint8_t BR = 0x00;

	for(segment_counter = 0; segment_counter <= 8; segment_counter++){
		GPIO_SetBits(GPIOA, GPIO_Pin_4);										//sclk on
		if((BR & 0x80) > 0x00){
			GPIO_SetBits(GPIOC, GPIO_Pin_0);									//1
		}else{
			GPIO_ResetBits(GPIOC, GPIO_Pin_0);									//0; always goes here
		}
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);										//sclk off
		BR <<= 1;
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_0);											//rclk on
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);											//rclk off

	GPIO_SetBits(GPIOC, GPIO_Pin_1);											//OE on
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);											//OE off

}
/**
 * Displays the number on 7seg
 * @param num Number to display
 * @param disp Display struct
 */
void SEG_DisplayNumber(uint8_t num, SEG_Disp* disp){
	int segment_counter;
	volatile uint8_t BR = disp->num_array[num];
	if(num > -1 && num < 10){
		for(segment_counter = 0; segment_counter<=8; segment_counter++){
			GPIO_SetBits(GPIOA, GPIO_Pin_4);								//srclk on
			if((BR & 0x80) > 0x00){
				GPIO_SetBits(GPIOC, GPIO_Pin_0);							//1
				Delay(5);
			}else{
				GPIO_ResetBits(GPIOC, GPIO_Pin_0);							//0
				Delay(5);
			}
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);								//srclk off
			BR <<= 1;
		}
		GPIO_SetBits(GPIOB, GPIO_Pin_0);									//rclk on
		Delay(5);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);									//rclok off

		GPIO_SetBits(GPIOC, GPIO_Pin_1);									//OE on
		Delay(5);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1);									//OE off
		disp->cnt = num;
	}
}
/**
 * Increment display
 * @param disp Display struct
 */
void SEG_Inc(SEG_Disp* disp){
	switch(disp->mode){
	case spin:
		if(disp->cnt == SPIN)
			disp->cnt = 0;
		else disp->cnt++;
		break;
	case field:
		if(disp->cnt == FIELDS)
			disp->cnt = 0;
		else disp->cnt++;
		break;
	}
	SEG_DisplayNumber(disp->cnt, disp);
}
/**
 * Seg mode set
 * @param mode spin or field
 * @param disp Display struct
 */
void SEG_ChangeMode(MODE_FLAG mode, SEG_Disp* disp){
	disp->mode = mode;
}
/**
 * Read display number
 * @param disp Display struct
 * @return Current number on the display
 */
uint8_t SEG_GetNumber(SEG_Disp* disp){
	return disp->cnt;
}
/**
 * Read diplay mode
 * @param disp Display struct
 * @return Current display mode
 */
MODE_FLAG SEG_GetMode(SEG_Disp* disp){
	return disp->mode;
}
