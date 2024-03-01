#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000

unsigned char backtrack = 0;
unsigned char trail_timer_high[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
unsigned char trail_timer_low[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
unsigned char trail_manoeuvre[20] = {101, 102, 103, 104, 105, 106, 107, 108, 109, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 100};
unsigned char *timer_high_pointer = &trail_timer_high[0];
unsigned char *timer_low_pointer = &trail_timer_low[0];
unsigned char *manoeuvre_pointer = &trail_manoeuvre[0];

void Timer0_init(void);
void __interrupt() ISR();

#endif
