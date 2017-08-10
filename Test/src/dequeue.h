/*
 * deque.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef DEQUEUE_H_
#define DEQUEUE_H_

#define MAX_LENGTH 20

typedef struct{
	int field;
	int spin;
	int status; //1 = set, -1 = not_set
}Ball_typedef;

typedef struct{
	Ball_typedef dequeue[MAX_LENGTH];
	int top;
	int bot;
	int counter;
}Dequeue;

void Dequeue_Init(Dequeue* deq);
int push(Ball_typedef ball, Dequeue* deq);
Ball_typedef pop(Dequeue* deq);
int Dequeue_GetCounter(Dequeue* deq);

#endif /* DEQUEUE_H_ */
