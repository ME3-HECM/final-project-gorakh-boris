#ifndef MANOEUVRES_H
#define	MANOEUVRES_H

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000

void card_red(DC_motor *mL, DC_motor *mR, unsigned char backtrack);
void card_green(DC_motor *mL, DC_motor *mR, unsigned char backtrack);
void card_blue(DC_motor *mL, DC_motor *mR, unsigned char backtrack);
void card_yellow(DC_motor *mL, DC_motor *mR, unsigned char backtrack);
void card_pink(DC_motor *mL, DC_motor *mR, unsigned char backtrack);
void card_orange(DC_motor *mL, DC_motor *mR, unsigned char backtrack);
void card_cyan(DC_motor *mL, DC_motor *mR, unsigned char backtrack);
void card_white(DC_motor *mL, DC_motor *mR);
void pick_card(DC_motor *mL, DC_motor *mR, unsigned char backtrack, unsigned char key);

#endif