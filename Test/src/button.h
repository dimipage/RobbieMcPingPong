/*
 * button.h
 *
 *  Created on: Aug 3, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"

#include "gpio.h"


typedef enum{
	START,	///prvobitno start zamisljen kao interrupt, depricated
	POWER
}BTN_EXTI;

void Btn_Init(BTN_EXTI btn, FunctionalState st); ///takodje i deinit ako je function state DISABLE

#endif /* BUTTON_H_ */
