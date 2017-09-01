/*
 * 7seg.h
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef SEG_H_
#define SEG_H_

#include "gpio.h"
#include "stm32f4xx_tim.h"

typedef enum{
	spin,
	field
}MODE_FLAG;

typedef struct{
	MODE_FLAG mode;
	volatile uint8_t cnt; 		//number on display; used for counting
	uint8_t num_array[10]; 		//numbers for 7seg
}SEG_Disp;

void SEG_Init(SEG_Disp* disp);
void SEG_DisplayNumber(uint8_t num, SEG_Disp* disp);
void SEG_Off(SEG_Disp* disp);
void SEG_Inc(SEG_Disp* disp);
void SEG_ChangeMode(MODE_FLAG mode, SEG_Disp* disp);
uint8_t SEG_GetNumber(SEG_Disp* disp);
MODE_FLAG SEG_GetMode(SEG_Disp* disp);
#endif /* SEG_H_ */
