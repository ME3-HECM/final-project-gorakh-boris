#ifndef _timers_H
#define _timers_H

#include <xc.h>
#include "dc_motor.h"
#include "manoeuvres.h"
#include "serial.h"

#define _XTAL_FREQ 64000000

unsigned char returning = 0;
unsigned char returnFlag = 0;
//example arrays for testing
    //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    //{101, 102, 103, 104, 105, 106, 107, 108, 109, 100, 201, 202, 203, 204, 205, 206, 207, 208, 209, 200};
    //{3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6};
    //{5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10};
    //{100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200};
unsigned char trail_timer_high[20] = {3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6};
unsigned char trail_timer_low[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char trail_manoeuvre[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//for the pointers and counter below, 0 is start of array, 19 for end of array, 20 for one after end of array
//when writing to array, pointer is moved AFTER writing
//when reading from array, pointer is moved BEFORE reading
//at beginning of program, the pointer is at 0
//after writing the final element (19), the pointer is moved forward to 20
//before reading the final element (19), the pointer is moved back to 19
//at end of program, the pointer is at 0
unsigned char *timer_high_pointer = &trail_timer_high[0];           //set to 0 or 20
unsigned char *timer_low_pointer = &trail_timer_low[0];             //set to 0 or 20
unsigned char *manoeuvre_pointer = &trail_manoeuvre[0];             //set to 0 or 20
unsigned char manoeuvre_count = 0;                              //can be set from 0 to 20 for different conditions

void Timer0_init(void);
void writeTrail(unsigned char *man);
void readTrail(unsigned char *tH, unsigned char *tL, unsigned char *man);
void returnToSender(DC_motor *mL, DC_motor *mR);
void __interrupt() ISR();

#endif
