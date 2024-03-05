/* 
 * File:   interrputs.h
 * Author: Gorakh
 *
 * Created on 05 March 2024, 14:24
 */

#ifndef _interrupts_H
#define	_interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void Interrupts_init(void);
void __interrupt(high_priority) HighISR();







#endif	/* INTERRPUTS_H */

