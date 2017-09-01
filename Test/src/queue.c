/*
 * deque.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "queue.h"
//mogle je lakse ovo da se odradi sa obicnim nizom

/**
 * Init queue; filling with empty balls
 * @param deq Queue struct
 */
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
 * Enqueue
 * @param ball Ball to enqueue
 * @param deq Queue to enqueue the ball
 */
int Enqueue(Ball_typedef ball, Queue* deq){
	if(deq->counter == MAX_LENGTH)
		return 0;
		//light the LED or else
	ball.status = 1;
	deq->dequeue[deq->top] = ball;
	deq->top = (deq->top + 1) % MAX_LENGTH;
	deq->counter++;
	return 1;
}

/**
 * Dequeue
 * @param deq Queue to dequeue from
 * @return Ball struct if one exists, if not returns empty ball with status -1
 */
Ball_typedef Dequeue(Queue* deq){
	int copy = deq->bot;
	if(deq->counter == 0){
		Ball_typedef b;
		b.status = -1;
		//light the LED or else
		return b;
	}
	deq->bot = (deq->bot + 1) % MAX_LENGTH;
	deq->counter--;
	return deq->dequeue[copy];
}
