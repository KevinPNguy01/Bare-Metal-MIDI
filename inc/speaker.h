/*
 * speaker.h
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#ifndef INC_SPEAKER_H_
#define INC_SPEAKER_H_

#include "tm4c123gh6pm.h"
#include "delays.h"
#include "midi.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

void speaker_init(void);
void speaker_play_notes(void);

#endif /* INC_SPEAKER_H_ */
