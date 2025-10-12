/*
 * timers.h
 *
 *  Created on: Oct 11, 2025
 *      Author: kevin
 */

#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

#include "tm4c123gh6pm.h"
#include "delays.h"
#include <stdint.h>
#include <stdbool.h>

extern volatile bool checkKeys;

void Timer0_Init(void);
void Timer0A_Handler(void);
void Timer0B_Handler();

#endif /* INC_TIMERS_H_ */
