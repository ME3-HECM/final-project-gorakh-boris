#ifndef MANOEUVRES_H
#define	MANOEUVRES_H

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000

void card_red(DC_motor *mL, DC_motor *mR, unsigned char backtrack);         //turn right 90
void card_green(DC_motor *mL, DC_motor *mR, unsigned char backtrack);       //turn left 90
void card_blue(DC_motor *mL, DC_motor *mR, unsigned char backtrack);        //turn 180
void card_yellow(DC_motor *mL, DC_motor *mR, unsigned char backtrack);      //reverse 1 square and turn right 90
void card_pink(DC_motor *mL, DC_motor *mR, unsigned char backtrack);        //reverse 1 square and turn left 90
void card_orange(DC_motor *mL, DC_motor *mR, unsigned char backtrack);      //turn right 135
void card_cyan(DC_motor *mL, DC_motor *mR, unsigned char backtrack);        //turn left 135
void card_white(DC_motor *mL, DC_motor *mR);                                //finish and return home
void pick_card(DC_motor *mL, DC_motor *mR, unsigned char backtrack, unsigned char key);

#endif