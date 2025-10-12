/*
 * keypad.h
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

extern char pressed_key;
extern const char keys[4][4];
extern int pressed[4][4];

void keypad_init(void);
void poll_keypad_handler(void);

#endif /* INC_KEYPAD_H_ */
