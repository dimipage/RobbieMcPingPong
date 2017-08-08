/*
 * pwm.c
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "pwm.h"

/**
 * Podesavanje impulsa PWM-a
 * @param pwmOC Kanal PWM-a
 * @param pulse Duzina impulsa (ako predje max vrednost preskalera linija je stalno HIGH)
 */
void PWM_SetPulse(TIM_OCInitTypeDef* pwmOC,uint32_t pulse){
	pwmOC->TIM_Pulse = pulse;
}
/**
 * Povecava duzinu impulsa
 * @param pwmOC Kanal PWM-a koji se povecava
 * @param pulse Inkrement
 */
void PWM_IncPulse(TIM_OCInitTypeDef* pwmOC,uint32_t inc){
	if((65535 - inc) > pwmOC->TIM_Pulse) //65535 max vrednost za preskaler samim tim i za impuls
		pwmOC->TIM_Pulse += inc;
}
/**
 * Smanjuje duzinu impulsa
 * @param pwmOC Kanal PWM-a koji se povecava
 * @param pulse Inkrement
 */
void PWM_DecPulse(TIM_OCInitTypeDef* pwmOC,uint32_t inc){
	if(pwmOC->TIM_Pulse > inc)
		pwmOC->TIM_Pulse -= inc;
}
/**
 * Podesavanje DutyCycle-a PWM kanala
 * @param pwmOC Kanal PWM-a
 * @param duty DutyCycle u procentima
 */
void PWM_SetDutyCycle(TIM_OCInitTypeDef* pwmOC, double duty, TIM_TimeBaseInitTypeDef* tim){
	if(duty > 0.0 && duty < 100.0)
		//pwmOC->TIM_Pulse = (uint32_t) ((65535 + 1) * duty) / 100 - 1;
		pwmOC->TIM_Pulse = (uint32_t) (tim->TIM_Period/100)*duty;
}

