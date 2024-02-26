#include <xc.h>
#include "buggy_lights.h"

void buggy_lights_init(void)
{
    //set up TRIS registers (0 for output)
    TRISAbits.TRISA0 = 0;               //BRAKE pin
    TRISDbits.TRISD0 = 0;               //HEADLAMPS pin
    TRISCbits.TRISC5 = 0;               //MAIN BEAM pin
    
    //set up initial LAT values
    LATAbits.LATA0 = 1;                 //BRAKE pin
    LATDbits.LATD0 = 1;                 //HEADLAMPS pin
    LATCbits.LATC5 = 1;                 //MAIN BEAM pin
}