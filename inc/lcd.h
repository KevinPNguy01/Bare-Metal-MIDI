/*
 * lcd.h
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "tm4c123gh6pm.h"
#include "delays.h"
#include <stdint.h>
#include <stdbool.h>

void lcd_init();
void lcd_write_instruction(char data);
void lcd_write_data(char data);

static void pulse_enable();
static void send_nibble(char data);
static void send_byte(char data);

#endif /* INC_LCD_H_ */
