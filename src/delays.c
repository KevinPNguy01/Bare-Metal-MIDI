/*
 * delays.c
 *
 *  Created on: Oct 12, 2025
 *      Author: kevin
 */

#include "delays.h"

// Flag indicating delay timer is finished
volatile bool delayFinished = false;

// Raise flag indicating delay timer is finished
void delayFinishedHandler() {
    delayFinished = true;
}

// Blocking delay with maximum 4096 microseconds
void delay(uint32_t us) {
    delayFinished = false;

    uint32_t ticks = (16 * us) - 1;   // Convert microseconds to ticks
    TIMER0_TAILR_R = ticks & 0xFFFF;  // Load 16-bit timer
    TIMER0_CTL_R |= 0x1;              // Start timer

    // Wait for completion
    while(!delayFinished);
}

// Blocking delay in microseconds
void delay_us(uint32_t us) {
    // Split into chunks of 4000 microseconds
    while (us >= 4000) {
        delay(4000);
        us -= 4000;
    }
    if (us) delay(us);
}

// Blocking delay in milliseconds
void delay_ms(uint32_t ms) {
    while (ms) {
        delay_us(1000);
        --ms;
    }
}
