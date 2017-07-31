/*
 * pwm.h
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef PWM_H_
#define PWM_H_

#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

void PWM_SetPulse(TIM_OCInitTypeDef* pwmOC, uint32_t pulse);
uint32_t PWM_GetPulse(TIM_OCInitTypeDef* pwmOC);

void PWM_IncPulse(TIM_OCInitTypeDef* pwmOC,uint32_t inc);
void PWM_DecPulse(TIM_OCInitTypeDef* pwmOC,uint32_t inc);

void PWM_SetDutyCycle(TIM_OCInitTypeDef* pwmOC, double duty);
double PWM_GetDutyCycle(TIM_OCInitTypeDef* pwmOC);
#endif /* PWM_H_ */
