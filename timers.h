#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000

unsigned int timer_count = 0;

void Timer0_init(void);
void __interrupt() ISR();

#endif
