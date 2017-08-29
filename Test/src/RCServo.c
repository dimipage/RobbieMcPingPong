/*
 * RCServo.c
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "RCServo.h"
#include "gpio.h"

#define INIT_ANGLE 95.0
#define INIT_ANGLE_PULSE 5004
#define ANGLE_STEP 18.2 //kolko perioda impulsa za jedan stepen
#define MIN_ANGLE_PULSE 3275.8
#define TOP_ANGLE_STEP 15 // u stepenima
#define BOT_ANGLE_STEP 20
#define SPIN_ANGLE_OFFSET 7
#define SPIN_ANGLE_OFFSET_BOT 2

TIM_TimeBaseInitTypeDef timer_test;///tajmer 4 za pwm za servo
TIM_OCInitTypeDef pwm;///pwm za servo

/**
 * Inicijalizacija TIM4, PWM CH1 na PB6
 */
void RC_Init(){
	NVIC_InitTypeDef nested_vector;

	timer_test.TIM_Prescaler = 30;
	timer_test.TIM_Period = 65535;
	timer_test.TIM_CounterMode = TIM_CounterMode_Up;
	timer_test.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_test.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timer_test);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	pwm.TIM_OCMode = TIM_OCMode_PWM1; 					//PWM1 = Set on compare match ====== PWM2 = Clear on compare match
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
/*
	pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
	6.5% duty cycle:     pulse_length = ((65535 + 1) * 6.5) / 100 - 1 = 4259
	7.5% duty cycle:     pulse_length = ((65535 + 1) * 7.5) / 100 - 1 = 4914                      +2?
	8.5% duty cycle:  	 pulse_length = ((65535 + 1) * 8.5) / 100 - 1 = 5569
	6552
*/
	RC_SetAngle(INIT_ANGLE);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	nested_vector.NVIC_IRQChannel = TIM4_IRQn;
	nested_vector.NVIC_IRQChannelPreemptionPriority = 1;
	nested_vector.NVIC_IRQChannelSubPriority = 0;
	nested_vector.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nested_vector);

	GPIOInit(GPIOB, GPIO_Pin_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

	//TIM_OC1Init(TIM4, &pwm); //mozda; test;
}
/**
 * Podesavanje ugla od 0 do 180
 * @param angle Stepen pomeraja od 0 do 180
 */
void RC_SetAngle(double angle){
	if(angle > 0.0 && angle < 180.0)
		pwm.TIM_Pulse = (uint32_t) (MIN_ANGLE_PULSE + (ANGLE_STEP * angle));
	TIM_OC1Init(TIM4, &pwm);
}
/**
 * Vraca ugao servo motora
 * @return Ugao servo motora
 */
double RC_GetAngle(){///koristi za testiranje kad se podesava potenciometrima
	return (pwm.TIM_Pulse - INIT_ANGLE_PULSE / ANGLE_STEP);
}
/**
 * Podesavanje ugla za ispaljivanje loptice
 * @param ball Struktura iz koje se cita polje
 */
void RC_SetField(Ball_typedef ball){
	extern ticks;
	int offset = 0; ///offset za ugao ako postoji spin
					///offset je hardkodiran i nije precizan, loptice se cudno ponasaju
	switch(ball.spin){
	case 2: //levi spin
		offset = -SPIN_ANGLE_OFFSET;
		break;
	case 3: //desni
		offset = SPIN_ANGLE_OFFSET;
		break;
	}

	if(ball.field >=1 && ball.field <=3){
		RC_SetAngle(80.0 + offset + ((ball.field - 1) * TOP_ANGLE_STEP));
	}
	else
		if(ball.field > 3 && ball.field <= 6){
			switch(ball.spin){
			case 2: //levi spin
				offset -= SPIN_ANGLE_OFFSET_BOT;
				break;
			case 3: //desni
				offset += SPIN_ANGLE_OFFSET_BOT;
				break;
			}
			RC_SetAngle(75.0 + offset + ((ball.field - 4) * BOT_ANGLE_STEP));
		}		//PWM_SetPulse(&pwm, (uint32_t) (MIN_ANGLE_PULSE + ((field - 3) * BOT_ANGLE_STEP * ANGLE_STEP)));
	if(ball.field == 0){
		int random;
		random = (ticks % 6) + 1;
		Ball_typedef copy;
		copy.spin = ball.spin;
		copy.field = random;
		RC_SetField(copy);
	}

	TIM_OC1Init(TIM4, &pwm);
}
