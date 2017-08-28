/*
 * state.h
 *
 *  Created on: Aug 15, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef STATE_H_
#define STATE_H_

typedef enum{
	init,
	set_mode,
	start,
	pause,
	power
}state_codes;

typedef enum{
	next,
	start_btn,
	reset_btn,
	power_btn
}ret_codes;

typedef struct{
	state_codes src_state;
	ret_codes   ret_code;
    state_codes dst_state;
}transition;

transition state_transition[] = {
	{init, next, set_mode},
	{set_mode, start_btn, start},
	{set_mode, reset_btn, set_mode},
	{set_mode, power_btn, power},
	{set_mode, next, start},
	{start, start_btn, pause},
	{start, reset_btn, set_mode},
	{start, power_btn, power},
	{start, next, set_mode},
	{pause, start_btn, start},
	{pause, reset_btn, set_mode},
	{pause, power_btn, power},
	{power, power_btn, init}
};
state_codes lookup_transitions(state_codes state, ret_codes ret);

#endif /* STATE_H_ */
