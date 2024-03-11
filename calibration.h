#ifndef CALIBRATION_H
#define	CALIBRATION_H

#include <xc.h>
#include "manoeuvres.h"

#define _XTAL_FREQ 64000000

void test_manoeuvres(DC_motor *mL, DC_motor *mR, unsigned char backtrack);

#endif