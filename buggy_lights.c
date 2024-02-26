#include <xc.h>
#include "buggy_lights.h"

void buggy_lights_init(void)
{
    //brake lights don't seem to be working, likely hardware failure
    //set up TRIS registers (0 for output)
    TRISDbits.TRISD4 = 0;               //BRAKE pin
    TRISHbits.TRISH1 = 0;               //HEADLAMPS pin
    TRISDbits.TRISD3 = 0;               //MAIN BEAM pin
    TRISFbits.TRISF0 = 0;               //TURN L pin
    TRISHbits.TRISH0 = 0;               //TURN R pin
    //set up initial LAT values
    LATDbits.LATD4 = 1;                 //BRAKE pin
    LATHbits.LATH1 = 1;                 //HEADLAMPS pin
    LATDbits.LATD3 = 1;                 //MAIN BEAM pin
    LATFbits.LATF0 = 1;                 //TURN L pin
    LATHbits.LATH0 = 1;                 //TURN R pin
}