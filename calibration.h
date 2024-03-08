/* 
 * File:   calibration.h
 * Author: boris
 *
 * Created on 29 February 2024, 15:22
 */

#ifndef CALIBRATION_H
#define	CALIBRATION_H

#include <xc.h>
#include "manoeuvres.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz

void test_manoeuvres(DC_motor *mL, DC_motor *mR, unsigned char backtrack);

#endif