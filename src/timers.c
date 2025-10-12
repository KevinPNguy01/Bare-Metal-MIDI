/*
 * timers.c
 *
 *  Created on: Oct 11, 2025
 *      Author: kevin
 */

#include "timers.h"

/**
 * Init Timer0A in one-shot mode for arbitrary delays
 * Init Timer0B in periodic mode for keypad polling
 */
void timer0_init(void) {
    SYSCTL_RCGCTIMER_R |= 0x1;      // Enable Timer0 clock
    while(!(SYSCTL_PRTIMER_R & 0x1));
    TIMER0_CTL_R = 0x0;             // Disable timer during setup
    TIMER0_CFG_R = 0x4;             // 16-bit timer

    // Timer 0A
    TIMER0_TAMR_R = 0x1;            // One-shot mode
    NVIC_EN0_R |= 1 << 19;          // Enable Timer0A IRQ in NVIC
    TIMER0_IMR_R |= 0x1;            // Enable timeout interrupt

    // Timer 0B
    TIMER0_TBMR_R = 0x2;            // Periodic mode
    TIMER0_TBPR_R = 5;              // Prescalar
    TIMER0_TBILR_R = 65306;         // Initcount
    NVIC_EN0_R |= (1 << 20);        // Enable Timer0B IRQ in NVIC
    TIMER0_IMR_R |= (1 << 8);       // Enable timeout interrupt
    TIMER0_CTL_R |= (1 << 8);       // Start Timer0B
}

void timer0A_handler(void) {
    TIMER0_ICR_R = 0x1; // Clear interrupt

    delay_finished_handler();
}

void timer0B_handler() {
    TIMER0_ICR_R |= (1 << 8);

    poll_keypad_handler();
}

void timer1_init() {
    // 1. Enable Timer1 clock
    SYSCTL_RCGCTIMER_R |= (1 << 1);        // Timer1
    while (!(SYSCTL_PRTIMER_R & (1 << 1))); // Wait for clock

    // 2. Disable timer during configuration
    TIMER1_CTL_R &= ~0x1;

    // 3. Configure as 16-bit timer
    TIMER1_CFG_R = 0x4;     // 16-bit mode
    TIMER1_TAMR_R = 0x2;    // Periodic timer mode

    // 4. Set interval for 125 us
    // 16 MHz clock -> 1 tick = 62.5 ns -> 125us / 62.5ns = 2000 ticks
    TIMER1_TAILR_R = 2000 - 1;

    // 5. Clear timeout flag
    TIMER1_ICR_R = 0x1;

    // 6. Enable timer and interrupt (optional)
    TIMER1_IMR_R |= 0x1;         // Enable timeout interrupt
    NVIC_EN0_R |= (1 << 21);     // Enable Timer1A IRQ in NVIC (IRQ 21)
    TIMER1_CTL_R |= 0x1;         // Start timer
}

void timer1A_handler() {
    TIMER1_ICR_R = 0x1; // Clear interrupt

    midi_sample_note();
}
