/*
 * pwm.c
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "pwm.h"
/**
 *
 * @param pwmOC
 * @param pulse
 */
void PWM_SetPulse(TIM_OCInitTypeDef* pwmOC,uint32_t pulse){
	pwmOC->TIM_Pulse = pulse;
}
/**
 *
 * @param pwmOC
 * @return
 */
uint32_t PWM_GetPulse(TIM_OCInitTypeDef* pwmOC){
	return pwmOC->TIM_Pulse;
}
/**
 *
 * @param pwmOC
 * @param pulse
 */
void PWM_IncPulse(TIM_OCInitTypeDef* pwmOC,uint32_t inc){
	if((65535 - inc) > pwmOC->TIM_Pulse)
		pwmOC->TIM_Pulse += inc;
}
/**
 *
 * @param pwmOC
 * @param pulse
 */
void PWM_DecPulse(TIM_OCInitTypeDef* pwmOC,uint32_t inc){
	if(pwmOC->TIM_Pulse > inc)
		pwmOC->TIM_Pulse -= inc;
}
/**
 *
 * @param
 */
void PWM_SetDutyCycle(TIM_OCInitTypeDef* pwmOC, double duty){
	if(duty > 0.0 && duty < 100.0)
		pwmOC->TIM_Pulse = (uint32_t) ((65535 + 1) * duty) / 100 - 1;
}
/**
 *
 * @param
 * @return
 */
double PWM_GetDutyCycle(TIM_OCInitTypeDef* pwmOC){
	return (pwmOC->TIM_Pulse + 1)*100/(65535 + 1);
}
