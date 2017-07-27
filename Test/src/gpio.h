/*
 * extra.h
 *
 *  Created on: Jul 27, 2017
 *      Author: Dusan Dimitrijevic
 */

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"


#ifndef EXTRA_H_
#define EXTRA_H_

#ifndef GPIO_Pin_0
#define GPIO_Pin_0		((uint16_t)0x0001)
#define GPIO_Pin_1		((uint16_t)0x0002)
#define GPIO_Pin_2		((uint16_t)0x0004)
#define GPIO_Pin_3		((uint16_t)0x0008)
#define GPIO_Pin_4		((uint16_t)0x0010)
#define GPIO_Pin_5		((uint16_t)0x0020)
#define GPIO_Pin_6		((uint16_t)0x0040)
#define GPIO_Pin_7		((uint16_t)0x0080)
#define GPIO_Pin_8		((uint16_t)0x0100)
#define GPIO_Pin_9		((uint16_t)0x0200)
#define GPIO_Pin_10		((uint16_t)0x0400)
#define GPIO_Pin_11		((uint16_t)0x0800)
#define GPIO_Pin_12		((uint16_t)0x1000)
#define GPIO_Pin_13		((uint16_t)0x2000)
#define GPIO_Pin_14		((uint16_t)0x4000)
#define GPIO_Pin_15		((uint16_t)0x8000)
#define GPIO_Pin_All	((uint16_t)0xFFFF)
#endif

typedef enum {
	TM_GPIO_Mode_IN = 0x00,
	TM_GPIO_Mode_OUT = 0x01,
	TM_GPIO_Mode_AF = 0x02,
	TM_GPIO_Mode_AN = 0x03,
} GPIOMode;

typedef enum {
	TM_GPIO_OType_PP = 0x00,
	TM_GPIO_OType_OD = 0x01
} GPIOOType;

typedef enum {
	TM_GPIO_Speed_Low = 0x00,
	TM_GPIO_Speed_Medium = 0x01,
	TM_GPIO_Speed_Fast = 0x02,
	TM_GPIO_Speed_High = 0x03
} GPIOSpeed;

typedef enum {
	TM_GPIO_PuPd_NOPULL = 0x00,
	TM_GPIO_PuPd_UP = 0x01,
	TM_GPIO_PuPd_DOWN = 0x02
} GPIOPuPd;

void InitGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIOPin, GPIOMode GPIOMode, GPIOOType GPIOOType, GPIOPuPd GPIOPuPd, GPIOSpeed GPIOSpeed);

void GPIOSetPinModeOut(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);
void GPIOSetPinModeIn(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);
void GPIOSetPinModeAF(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);

void GPIOSetOutputTypePP(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);
void GPIOSetOutputTypeOD(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);

void GPIOSetSpeed(GPIO_TypeDef* GPIOx, uint16_t GPIOPin, GPIOSpeed GPIOSpeed);

void GPIOSetPu(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);
void GPIOSetPd(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);
void GPIOSetNoPuPd(GPIO_TypeDef* GPIOx, uint16_t GPIOPin);

#endif /* EXTRA_H_ */
