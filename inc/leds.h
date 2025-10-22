/*
 * leds.h
 *
 *  Created on: Oct 22, 2025
 *      Author: kevin
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include "tm4c123gh6pm.h"
#include <stdint.h>

extern void leds_init(void);
extern void leds_turn_all_off(void);
extern void leds_turn_on_one(uint8_t i);

#endif /* INC_LEDS_H_ */
