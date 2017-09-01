/*
 * state.c
 *
 *  Created on: Aug 15, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "stm32f4xx_rcc.h"
#include "stepper.h"
#include "button.h"
#include "led.h"
#include "gpio.h"
#include "DCMotor.h"
#include "RCServo.h"
#include "seg.h"
#include "state.h"
#include "queue.h"

extern volatile uint32_t ticks = 0;		//systick counter
extern int8_t start_mode_flag = 0;		//flag to set if shooting (start) state has been requested
extern int8_t start_flag = -1;			//flag to set if pause/start state change has been requested
extern int8_t power_btn_flag = 0;		//flag for power off; not yet used
int instant_start;						//flag for instant start after the shooting (start) state has ended, sets to 0 if first ball after finishing is set

#define DEBOUNCE 40						//debounce time
volatile int8_t exti_flag = 0;			//flag for debounce timer interrupt
volatile int exti_cnt = 0; 				//button click time counter
volatile int8_t state_reset = 0;		//reset flag, sets when start button is held for 3s

//states
int init_state();
int set_mode_state();
int start_state();
int pause_state();
int power_state();

//states f ptr
int (*state[])(void) = { init_state, set_mode_state, start_state, pause_state, power_state };
#define ENTRY_STATE init				//state machine starting state
#define EXIT_STATE power				//state machine ending state

ret_codes rc;
GPIO_InitTypeDef btn_set, btn_mode, btn_start;
GPIO_InitTypeDef sen_top, sen_bottom;
SEG_Disp seg_disp;
Queue deq;

int deq_bot; 							//used to store starting ball pointer to continue from when instant start is requested
int deq_counter_temp = 0;				//used to store ball counter for current setting

#define RELEASE
/**
 * Starting state for initializing components, setting clock, etc...
 * @return Return state for entering the next one
 */
int init_state(){
	//clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//Sensors GPIO
	sen_top = GPIOInit(GPIOB, GPIO_Pin_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	sen_bottom = GPIOInit(GPIOB, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//LED
	LED_Init();						//onboard LED
	Btn_Init(POWER, ENABLE);		//power_off
	SEG_Init(&seg_disp); 			//7seg
	SysTickInit(1000); 				//systick cnt 1ms
	DC_Init();						//dc motors
	RC_Init();						//servo
#ifdef RELEASE
	Step_Init();					//stepper
	Delay(250);						//delay to postpone two sequential stepbacks for 0.5s (0.25s in Step.Init)
	Step_Stepback();				//unload ball
#endif

	Queue_Init(&deq); 				//queue
	return next;
}
/**
 * State for setting ball parameters
 * @return Return state for entering the next one
 */
int set_mode_state(){
	Ball_typedef ball;

	RC_SetAngle(95.0); 				//init angle
	DC_Init();						//turn off the DC
	//set & mode btns GPIO
	btn_set = GPIOInit(GPIOC, GPIO_Pin_10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	btn_mode = GPIOInit(GPIOC, GPIO_Pin_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//btn start, interrupt on tim3
	btn_start = GPIOInit(GPIOC, GPIO_Pin_12, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_High_Speed);
	//seg num & state
	SEG_DisplayNumber(0, &seg_disp);
	SEG_ChangeMode(spin, &seg_disp);

	if(state_reset == 1){
		deq.bot = deq.top = 9;										//swap literal to (sizeof(deq.dequeue) / sizeof(deq.dequeue[0])) / 2 - 1;
		deq.counter = 0;
		state_reset = 0;
	}
	instant_start = 1;
	while(1){
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)){				//mode btn
			SEG_Inc(&seg_disp);
			Delay(500);												//delay used to slow down the setting when set button is held
		}
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)){				//set btn
			SEG_Off(&seg_disp);
			Delay(250);												//hold the 7seg off for 250ms to visually verify when set button is pressed
			if(SEG_GetMode(&seg_disp) == spin){ 					//spin set
				ball.spin = SEG_GetNumber(&seg_disp);
				SEG_DisplayNumber(0, &seg_disp);
				SEG_ChangeMode(field, &seg_disp);
			}
			else if(SEG_GetMode(&seg_disp) == field){				//field set
				ball.field = SEG_GetNumber(&seg_disp);
				SEG_DisplayNumber(0, &seg_disp);
				SEG_ChangeMode(spin, &seg_disp);
				if(Enqueue(ball, &deq) == 0)
					LED_LightOn();									//LED on when max number of set balls is reached
				instant_start = 0;									//even one ball set disables the instant start
			}
			Delay(500);												//same delay as the one above
		}
		if(start_mode_flag > 0){									//if start btn is pressed
			if(instant_start == 1){
				deq.bot = 9;										//set the pointer to the first ball
				deq.counter = deq_counter_temp;						//set the ball counter to the previous setting
			}else{
				deq_counter_temp = deq.counter;						//set the current setting ball counter to temp variable
			}
			start_mode_flag = 0;									//flag reset
			return start_btn;
		}
		if(power_btn_flag == 1){									//power btn press
			power_btn_flag = 0;										//flag reset
			return power_btn;
		}
		if(state_reset == 1)										//reset button press
			return reset_btn;
	}
}
/**
 * State for ball shooting, fires the same sequence of set balls until there is none left in the loader
 * @return Return state for entering the next one
 */
int start_state(){
	Delay(500);														//delay to slow down the state machine; not necessarily needed
	start_flag = 0;													//flag reset
	int bad_sensor_fix = 3;											//top sensor misbehaving; stepback 3 times to eventual read the balls if present
	while(bad_sensor_fix > 0){
		LED_LightOff();
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) > 0){		//top sensor
			Ball_typedef ball;
			bad_sensor_fix = 3;										//reset counter to 3 if any ball is read
			ball = Dequeue(&deq);

			if(ball.status == -1)									//invalid ball; used for testing
				return reset_btn;									//not necessarily needed

			RC_SetField(ball);
			Delay(50);												//delaying for servo to stabilize and then set DC speeds

			DC_Init();
			//novo
			if(ball.field > 3)
				DC_SetBotSpeed(&ball);
			else
				DC_SetTopSpeed(&ball);

			Delay(300); 											//delaying to compensate slower acceleration of DCs
			Step_Step();											//firing the ball
			Delay(1000);
			//check for request
			if(deq.counter == 0){
				deq.counter = deq_counter_temp;
				deq.bot = 9;  										//sizeof(deq.dequeue)/sizeof(deq.dequeue[0]) / 2 - 1;
			}
			if(power_btn_flag == 1){
				power_btn_flag = 0;											//flag reset
				return power_btn;
			}
			if(state_reset == 1)
				return reset_btn;

			if(start_flag == 1){
				if(state_reset == 1)
					return reset_btn;
				start_flag = 0;
				return start_btn;
			}
		}
		Step_Stepback();
		Delay(500);													//wait the ball to calm down
		bad_sensor_fix--;
	}
	return next;
}
/**
 * Pause state
 * @return Return state for entering the next one
 */
int pause_state(){
	LED_LightOff();
	DC_SetSpeed(MOT1, 99);									//turn off DC
	DC_SetSpeed(MOT2, 99);
	DC_SetSpeed(MOT3, 99);
	RC_SetAngle(95);
	while(1){
		if(power_btn_flag == 1){
			power_btn_flag = 0;								//reset flag
			return power_btn;
		}
		if(state_reset == 1)
			return reset_btn;

		if(start_flag == 1){								//new
			if(state_reset == 1)
				return reset_btn;
			start_flag = 0;
			return start_btn;
		}
	}
 }
/**
 * Power off state; power mode to be decided
 * @return Return state for entering the next one
 */
int power_state(){
	// turn standby mode
//event or interrupt?
	//when wkup return power_btn
}

int main() {

 	state_codes cur_state = ENTRY_STATE;
    int (* state_fun)(void);

    while(1) {
        state_fun = state[cur_state];
        rc = state_fun();														//state call
        if (EXIT_STATE == cur_state)
            break;
        cur_state = lookup_transitions(cur_state, rc);							//state transition
    }

    return 0;
}
/**
 * Lookup transitions and get the next state
 * @param state out state
 * @param ret return state
 * @return next state
 */
state_codes lookup_transitions(state_codes state, ret_codes ret){
	int counter;
	for(counter = 0; counter < 12; counter++){
		if(state_transition[counter].src_state == state && state_transition[counter].ret_code == ret)
			return state_transition[counter].dst_state;
	}
	return 0;
}

void SysTick_Handler(){
	ticks++;
}
void SysTickInit (uint16_t frequency){
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);
	SysTick_Config (RCC_Clocks.HCLK_Frequency / frequency);						//1000 for 1ms systick
	NVIC_SetPriority(SysTick_IRQn, 0);
}
void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)){
			(exti_cnt > 0) ? (exti_cnt-=1) : (exti_cnt = 0);
			if(exti_flag > 0){
				if(exti_cnt < 59998 && exti_cnt > 800){							//START PRESS
					start_mode_flag = 1;										//request for shooting (start) state
					(start_flag > 0) ? (start_flag = 0) : (start_flag = 1);		//start/stop flag set
				}
				else if(exti_cnt >= 59999){										//RESET PRESS
					state_reset = 1;											//request for reset
					start_mode_flag = 0;										//flag reset
					start_flag = 1;												//flag reset
				}
				exti_flag = 0;													//reset flag
			}
		}else{
			(exti_cnt < 60000) ? (exti_cnt += 1) : (exti_cnt = 60000);			//debounce increment
			exti_flag = 1;														//
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
void TIM4_IRQHandler(){
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void Delay(uint32_t t){
	  uint32_t start, end;
	  start = ticks;
	  end = start + t;
	  if(start < end)
	    while ((ticks >= start) && (ticks < end)) { }
	  else
	    while ((ticks >= start) || (ticks < end)) { };
}
