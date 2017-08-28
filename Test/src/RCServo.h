/*
 * RCServo.h
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */
#include "queue.h"

#ifndef RCSERVO_H_
#define RCSERVO_H_

void RC_Init();
void RC_SetAngle(double angle);
double RC_GetAngle();
void RC_SetField(Ball_typedef ball);

#endif /* RCSERVO_H_ */
