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

#define SPIN 4
#define FIELDS 6


void SEG_Init();
void SEG_DisplayNumber(uint8_t num);
void SEG_Inc();
#endif /* SEG_H_ */
