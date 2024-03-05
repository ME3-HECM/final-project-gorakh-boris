/* 
 * File:   interrputs.h
 * Author: Gorakh
 *
 * Created on 05 March 2024, 14:24
 */

#ifndef INTERRPUTS_H
#define	INTERRPUTS_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void interrupts_init(void);
void __interrupt(high_priority) HighISR();







#endif	/* INTERRPUTS_H */

