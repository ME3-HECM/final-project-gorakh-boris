#ifndef _timers_H
#define _timers_H

#include <xc.h>
#include "manoeuvres.h"
#include "serial.h"
#include "color.h"
#include "buggy_lights.h"

#define _XTAL_FREQ 64000000
#define SERIAL_FEEDBACK 0

unsigned char returning = 0;
unsigned char return_flag = 0;
unsigned char lost_flag = 0;
/*******************************************************************************
 * Example arrays for testing
 * {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 * {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
 * {3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6};
 * {5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10, 5, 10};
 * {101, 102, 103, 104, 105, 106, 107, 108, 109, 100, 201, 202, 203, 204, 205, 206, 207, 208, 209, 200};
 * {100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200, 100, 200};
*******************************************************************************/
unsigned char trail_timer_high[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char trail_timer_low[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char trail_manoeuvre[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/*******************************************************************************
 * For the pointers and counter below, 0 is start of array, 19 for end of array, 20 for one after end of array
 * When writing to array, pointer is moved AFTER writing
 * When reading from array, pointer is moved BEFORE reading
 * At beginning of program, the pointer is at 0
 * AFTER writing the final element (19), the pointer is moved forward to 20
 * BEFORE reading the final element (19), the pointer is moved back to 19
 * At end of program, the pointer is at 0
*******************************************************************************/
unsigned char *timer_high_pointer = &trail_timer_high[0];       //set to 0 or 20
unsigned char *timer_low_pointer = &trail_timer_low[0];         //set to 0 or 20
unsigned char *manoeuvre_pointer = &trail_manoeuvre[0];         //set to 0 or 20
unsigned char manoeuvre_count = 0;                              //set to 0 to 20

void Timer0_init(void);

void start_timer(void);
void stop_timer(void);

void read_timer(unsigned char *tH, unsigned char *tL);
void write_timer(unsigned char tH, unsigned char tL);
void reset_timer(void);

void read_trail(unsigned char *tH, unsigned char *tL, unsigned char *man);
void write_trail(unsigned char tH, unsigned char tL, unsigned char man);

void forward_navigation(DC_motor *mL, DC_motor *mR, HSV_val *p1, RGBC_val *p2);
void return_to_sender(DC_motor *mL, DC_motor *mR);

void __interrupt() ISR();

#endif