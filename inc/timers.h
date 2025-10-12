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
#include "keypad.h"
#include "midi.h"
#include <stdint.h>
#include <stdbool.h>

extern volatile bool checkKeys;

void timer0_init(void);
void timer0A_handler(void);
void timer0B_handler(void);

void timer1_init(void);
void timer1A_handler(void);


#endif /* INC_TIMERS_H_ */
