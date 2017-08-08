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

void SEG_Init();
void SEG_DisplayNumber(uint8_t num);
void SEG_Inc();
void SEG_ChangeMode(MODE_FLAG m);
#endif /* SEG_H_ */
