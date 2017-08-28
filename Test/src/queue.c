/*
 * deque.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "queue.h"

void Queue_Init(Queue* deq){
	int count;
	Ball_typedef ball;
	ball.field = 0;
	ball.spin = 0;
	ball.status = 0;
	deq->top = (int) (MAX_LENGTH-1) / 2;
	deq->bot = (int) (MAX_LENGTH-1) / 2;
	deq->counter = 0 ;
	for(count = 0; count < MAX_LENGTH; count++)
		deq->dequeue[count] = ball;
}

/**
 * Push na red
 * @param ball Struktura koja se ubacuje u red
 */
int push(Ball_typedef ball, Queue* deq){
	if(deq->counter == MAX_LENGTH)
		return 0;
		//svetli dioda ili tako nesto

	ball.status = 1;
	deq->dequeue[deq->top] = ball;
	deq->top = (deq->top + 1) % MAX_LENGTH;
	deq->counter++;
	return 1;
}

/**
 * Pop iz reda
 * @return Vraca strukturu kuglice ako postoji
 */
Ball_typedef pop(Queue* deq){
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
