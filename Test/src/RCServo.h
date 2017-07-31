/*
 * RCServo.h
 *
 *  Created on: Jul 31, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "pwm.h"

#ifndef RCSERVO_H_
#define RCSERVO_H_

void RC_Init();
void RC_SetAngle(double angle);
double RC_GetAngle();

#endif /* RCSERVO_H_ */
