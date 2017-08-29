/*
 * deque.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Dusan Dimitrijevic
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define MAX_LENGTH 20

typedef struct{
	int field;
	int spin;
	int status; //1 = set, -1 = not_set
}Ball_typedef;

typedef struct{
	Ball_typedef dequeue[MAX_LENGTH]; ///dogovoreno staticki na 20 loptica
	int top;
	int bot;
	int counter;
}Queue;

void Queue_Init(Queue* deq);

int Enqueue(Ball_typedef ball, Queue* deq);
Ball_typedef Dequeue(Queue* deq);

#endif /* QUEUE_H_ */
