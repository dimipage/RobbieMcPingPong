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

//flagovi
extern volatile uint32_t ticks = 0;
extern int8_t start_mode_flag = 0;
extern int8_t start_flag = -1;
extern int8_t power_btn_flag = 0;	//flag za power off
int instant_start;	//flag za proveru instant starta

//debounce
#define DEBOUNCE 40					//debounce time
volatile int8_t exti_flag = 0;		//flag za tim interrupt da radi debounce
volatile int exti_cnt = 0; 			//brojac za reset ili start click
volatile int8_t state_reset = 0;	//flag za reset

//states
int init_state();
int set_mode_state();
int start_state();
int pause_state();
int power_state();

//states fptr
int (*state[])(void) = { init_state, set_mode_state, start_state, pause_state, power_state };

//defines
#define ENTRY_STATE init
#define EXIT_STATE power

ret_codes rc;
GPIO_InitTypeDef btn_set, btn_mode, btn_start;
GPIO_InitTypeDef sen_top, sen_bottom;
SEG_Disp seg_disp;
Queue deq;

int deq_bot; //kod pauziranja i startovanja kao i posle zavrsetka igre pamti pocetni pointer
int deq_counter_temp = 0;	//za pamcenje broja setovanih loptica


int init_state(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//senzori GPIO
	sen_top = GPIOInit(GPIOB, GPIO_Pin_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	sen_bottom = GPIOInit(GPIOB, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//led
	LED_Init();				//led na plocici
	Btn_Init(POWER, ENABLE);//dugme power_off
	SEG_Init(&seg_disp); 	//7seg
	SysTickInit(1000); 		//systick cnt 1ms
	DC_Init();				//dc motori init
	RC_Init();				//servo
	Step_Init();			//stepper
	Delay(500);
	Step_Stepback();		//unload ball
	Delay(200);
	Queue_Init(&deq); 	//queue

	return next;
}

int set_mode_state(){
	Ball_typedef ball;

	RC_SetAngle(95.0); //init angle
	DC_Init();//gasi motore
	//set i mode btns GPIO
	btn_set = GPIOInit(GPIOC, GPIO_Pin_10, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	btn_mode = GPIOInit(GPIOC, GPIO_Pin_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz);
	//dugme start, interrupt kroz tim3
	btn_start = GPIOInit(GPIOC, GPIO_Pin_12, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_High_Speed);
	//seg num & state
	SEG_DisplayNumber(0, &seg_disp);
	SEG_ChangeMode(spin, &seg_disp);

	if(state_reset == 1){
		deq.bot = deq.top = 9; //zameni literal sa (sizeof(deq.dequeue) / sizeof(deq.dequeue[0])) / 2 - 1;
		deq.counter = 0;
		state_reset = 0;
	}
	instant_start = 1;
	while(1){
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)){//mode dugme
			SEG_Inc(&seg_disp);
			Delay(500);
		}
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)){//set dugme
			SEG_Off(&seg_disp);
			Delay(250);
			if(SEG_GetMode(&seg_disp) == spin){ // spin set
				ball.spin = SEG_GetNumber(&seg_disp);
				SEG_DisplayNumber(0, &seg_disp);
				SEG_ChangeMode(field, &seg_disp);
			}
			else if(SEG_GetMode(&seg_disp) == field){// field set
				ball.field = SEG_GetNumber(&seg_disp);
				SEG_DisplayNumber(0, &seg_disp);
				SEG_ChangeMode(spin, &seg_disp);
				if(push(ball, &deq) == 0)
					LED_LightOn();
				instant_start = 0;	//ako je setovana makar jedna loptica
									//instant start ne radi
			}
			Delay(500);
		}
		if(start_mode_flag > 0){	//start btn press
			if(instant_start == 1){
				deq.bot = 9;
				deq.counter = deq_counter_temp;
			}else{
				deq_counter_temp = deq.counter;
			}
			start_mode_flag = 0;
			return start_btn;
		}
		if(power_btn_flag == 1){	//power btn press
			power_btn_flag = 0;
			return power_btn;
		}
		if(state_reset == 1)	//reset button press
			return reset_btn;
	}
}

int start_state(){
	Delay(500);
	start_flag = 0;
	int bad_sensor_fix = 3;		//senzor za ocitavanje loptica nekad promasi
	// iz tog razloga stepper se vrti max 3 puta unazad da proba da ocita lopticu ako je ima
	while(bad_sensor_fix > 0){
		LED_LightOff();
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) > 0){//senzor 2
			Ball_typedef ball;
			bad_sensor_fix = 3;	//reset flag
			ball = pop(&deq);

			if(ball.status == -1)//nevalidna loptica
				return reset_btn;//ovo je samo debug, brisi u release

			RC_SetField(ball);
			Delay(50);

			if(ball.field > 3)
				DC_SetBotSpeed(&ball);
			else
				DC_SetTopSpeed(&ball);

			Delay(300); //dok se zalete motori
			Step_Step();//propusti lopticu
			Delay(1000);
			if(deq.counter == 0){
				deq.counter = deq_counter_temp;
				deq.bot = 9;  //sizeof(deq.dequeue)/sizeof(deq.dequeue[0]) / 2 - 1;
			}
		}
		Step_Stepback();
		bad_sensor_fix--;
	}

	if(power_btn_flag == 1){
		power_btn_flag = 0;
		return power_btn;
	}
	if(state_reset == 1)
		return reset_btn;

	if(start_flag == 1){
		start_flag = 0;
		return start_btn;
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0 && deq.counter > 1){
		LED_LightOn();
		return next;
	}
	return next;
}
int pause_state(){
	LED_LightOff();
	DC_SetSpeed(MOT1, 98);
	DC_SetSpeed(MOT2, 98);
	DC_SetSpeed(MOT3, 98);
	RC_SetAngle(95);
	while(1){
		if(power_btn_flag == 1){
			power_btn_flag = 0;
			return power_btn;
		}
		if(state_reset == 1)
			return reset_btn;

		if(start_flag == 1)
			return start_btn;
	}
 }
int power_state(){
	// turn standby mode

	//when wkup return power_btn
}

int main() {
    state_codes cur_state = ENTRY_STATE;
    int (* state_fun)(void);

    while(1) {
        state_fun = state[cur_state];
        rc = state_fun();
        if (EXIT_STATE == cur_state)
            break;
        cur_state = lookup_transitions(cur_state, rc);
    }

    return 0;
}

state_codes lookup_transitions(state_codes state, ret_codes ret){
	int i;
	for(i = 0; i < 12; i++){
		if(state_transition[i].src_state == state && state_transition[i].ret_code == ret)
			return state_transition[i].dst_state;
	}
	return 0;
}

void SysTick_Handler(){
	ticks++;
}
void SysTickInit (uint16_t frequency){
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);
	SysTick_Config (RCC_Clocks.HCLK_Frequency / frequency);
	NVIC_SetPriority(SysTick_IRQn, 0); //priority veci od EXTI
}
void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)){
			(exti_cnt > 0) ? (exti_cnt-=1) : (exti_cnt = 0);
			if(exti_flag > 0){
				if(exti_cnt < 59998 && exti_cnt > 800){//START PRESS
					start_mode_flag = 1;
					(start_flag > 0) ? (start_flag = 0) : (start_flag = 1);
				}
				else if(exti_cnt >= 59999){		//RESET
					state_reset = 1;
					start_mode_flag = 0;
					start_flag = 1;
				}
				exti_flag = 0;
			}
		}else{
			(exti_cnt < 60000) ? (exti_cnt += 1) : (exti_cnt = 60000);
			exti_flag = 1;
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
