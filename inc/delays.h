/*
 * delays.h
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#ifndef INC_DELAYS_H_
#define INC_DELAYS_H_

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

extern volatile bool delayFinished;

void delay(uint32_t us);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void delayFinishedHandler();

#endif /* INC_DELAYS_H_ */
