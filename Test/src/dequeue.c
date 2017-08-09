/*
 * deque.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "dequeue.h"



void Dequeue_Init(Dequeue* deq){
	deq->top = (int) (MAX_LENGTH-1) / 2 + 1;
	deq->bot = (int) (MAX_LENGTH-1) / 2;
	deq->counter = 0 ;
}

/**
 * Push na red
 * @param ball Struktura koja se ubacuje u red
 */
int push(Ball_typedef ball, Dequeue* deq){
	if(deq->counter == MAX_LENGTH)
		return 0;
		//svetli dioda ili tako nesto

	ball.status = 1;
	deq->top = (deq->top + 1) % MAX_LENGTH;
	deq->dequeue[deq->top] = ball;
	deq->counter++;
	return 1;
}

/**
 * Pop iz reda
 * @return Vraca strukturu kuglice ako postoji
 */
Ball_typedef pop(Dequeue* deq){
	int copy = deq->bot;
	if(deq->counter == 0){
		Ball_typedef b;
		b.status = -1;
		//svetli dioda ili tako nesto
		return b;
	}
	deq->bot = (deq->bot + 1) % MAX_LENGTH;
	deq->counter--;
	return deq->dequeue[copy];
}
