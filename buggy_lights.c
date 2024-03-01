#include <xc.h>
#include "buggy_lights.h"

void buggy_lights_init(void)
{
    //brake lights don't seem to be working, likely hardware failure
    //set up TRIS registers (0 for output)
    TRISDbits.TRISD4 = 0;               //BRAKE pin (DOESN'T WORK)
    TRISHbits.TRISH1 = 0;               //HEADLAMPS pin
    TRISDbits.TRISD3 = 0;               //MAIN BEAM pin
    TRISFbits.TRISF0 = 0;               //TURN L pin
    TRISHbits.TRISH0 = 0;               //TURN R pin
    //set up initial LAT values
    LATDbits.LATD4 = 1;                 //BRAKE pin (DOESN'T WORK)
    LATHbits.LATH1 = 1;                 //HEADLAMPS pin
    LATDbits.LATD3 = 1;                 //MAIN BEAM pin
    LATFbits.LATF0 = 1;                 //TURN L pin
    LATHbits.LATH0 = 1;                 //TURN R pin
    
    //initialise the two LEDs on the clicker board
    //set up TRIS registers (0 for output)
    TRISDbits.TRISD7 = 0;
    TRISHbits.TRISH3 = 0;
    //set up initial LAT values
    LATDbits.LATD7 = 1;
    LATHbits.LATH3 = 1;
    
    //initialise the three LED pins on the colour board
    
    //colour click mounted on mikroBUS1 on clicker board
        //set up TRIS registers (0 for output)
        TRISAbits.TRISA0 = 0;               //red LED pin
        TRISCbits.TRISC2 = 0;               //green LED pin
        TRISDbits.TRISD0 = 0;               //blue LED pin
        //set up initial LAT values
        LATAbits.LATA0 = 1;                 //red LED pin
        LATCbits.LATC2 = 1;                 //green LED pin
        LATDbits.LATD0 = 1;                 //blue LED pin
        
    //colour click mounted on mikroBUS2 on clicker board
        //set up TRIS registers (0 for output)
        TRISAbits.TRISA1 = 0;               //red LED pin
        TRISCbits.TRISC6 = 0;               //green LED pin
        TRISHbits.TRISH2 = 0;               //blue LED pin
        //set up initial LAT values
        LATAbits.LATA1 = 1;                 //red LED pin
        LATCbits.LATC6 = 1;                 //green LED pin
        LATHbits.LATH2 = 1;                 //blue LED pin

    //colour click mounted on mikroBUS1 on buggy
        //set up TRIS registers (0 for output)
        TRISGbits.TRISG1 = 0;               //red LED pin
        TRISAbits.TRISA4 = 0;               //green LED pin
        TRISFbits.TRISF7 = 0;               //blue LED pin
        //set up initial LAT values
        LATGbits.LATG1 = 1;                 //red LED pin
        LATAbits.LATA4 = 1;                 //green LED pin
        LATFbits.LATF7 = 1;                 //blue LED pin
    
    //colour click mounted on mikroBUS2 on buggy
        //set up TRIS registers (0 for output)
        TRISGbits.TRISG0 = 0;               //red LED pin
        TRISEbits.TRISE7 = 0;               //green LED pin
        TRISAbits.TRISA3 = 0;               //blue LED pin
        //set up initial LAT values
        LATGbits.LATG0 = 1;                 //red LED pin
        LATEbits.LATE7 = 1;                 //green LED pin
        LATAbits.LATA3 = 1;                 //blue LED pin
}

void lights_flashing(void)
{
    LATDbits.LATD4 = !LATDbits.LATD4;       //flash brake lights (DOESN'T WORK)
    LATDbits.LATD3 = !LATDbits.LATD3;       //flash main beam
    LATFbits.LATF0 = !LATFbits.LATF0;       //flash left indicators
    LATHbits.LATH0 = !LATHbits.LATH0;       //flash right indicators
}