#include <xc.h>
#include "buggy_lights.h"

void buggy_lights_init(void)
{
    //initialise car lights on the buggy
        //set up TRIS registers (0 for output)
        TRISDbits.TRISD4 = 0;               //BRAKE pin
        TRISHbits.TRISH1 = 0;               //HEADLAMPS pin
        TRISDbits.TRISD3 = 0;               //MAIN BEAM pin
        TRISFbits.TRISF0 = 0;               //TURN L pin
        TRISHbits.TRISH0 = 0;               //TURN R pin
        //set up initial LAT values
        LATDbits.LATD4 = 0;                 //BRAKE pin
        LATHbits.LATH1 = 0;                 //HEADLAMPS pin
        LATDbits.LATD3 = 0;                 //MAIN BEAM pin
        LATFbits.LATF0 = 0;                 //TURN L pin
        LATHbits.LATH0 = 0;                 //TURN R pin
    
    //initialise the tricolour LED pins on the colour board
    //colour click mounted on buggy
        //colour click mounted on mikroBUS1 on buggy
            //set up TRIS registers (0 for output)
            TRISGbits.TRISG1 = 0;               //red LED pin
            TRISAbits.TRISA4 = 0;               //green LED pin
            TRISFbits.TRISF7 = 0;               //blue LED pin
            //set up initial LAT values
            LATGbits.LATG1 = 0;                 //red LED pin
            LATAbits.LATA4 = 0;                 //green LED pin
            LATFbits.LATF7 = 0;                 //blue LED pin

        //colour click mounted on mikroBUS2 on buggy
            //set up TRIS registers (0 for output)
            TRISGbits.TRISG0 = 0;               //red LED pin
            TRISEbits.TRISE7 = 0;               //green LED pin
            TRISAbits.TRISA3 = 0;               //blue LED pin
            //set up initial LAT values
            LATGbits.LATG0 = 0;                 //red LED pin
            LATEbits.LATE7 = 0;                 //green LED pin
            LATAbits.LATA3 = 0;                 //blue LED pin
    
    /*
    //colour click mounted on clicker board
        //colour click mounted on mikroBUS1 on clicker board
            //set up TRIS registers (0 for output)
            TRISAbits.TRISA0 = 0;               //red LED pin
            TRISCbits.TRISC2 = 0;               //green LED pin
            TRISDbits.TRISD0 = 0;               //blue LED pin
            //set up initial LAT values
            LATAbits.LATA0 = 0;                 //red LED pin
            LATCbits.LATC2 = 0;                 //green LED pin
            LATDbits.LATD0 = 0;                 //blue LED pin

        //colour click mounted on mikroBUS2 on clicker board
            //set up TRIS registers (0 for output)
            TRISAbits.TRISA1 = 0;               //red LED pin
            TRISCbits.TRISC6 = 0;               //green LED pin
            TRISHbits.TRISH2 = 0;               //blue LED pin
            //set up initial LAT values
            LATAbits.LATA1 = 0;                 //red LED pin
            LATCbits.LATC6 = 0;                 //green LED pin
            LATHbits.LATH2 = 0;                 //blue LED pin
    */
}

void toggle_brake_lights(void)
{
    LATDbits.LATD4 = !LATDbits.LATD4;
}

void toggle_main_beam(void)
{
    LATDbits.LATD3 = !LATDbits.LATD3;
}

void toggle_left_indicators(void)
{
    LATFbits.LATF0 = !LATFbits.LATF0;
}

void toggle_right_indicators(void)
{
    LATHbits.LATH0 = !LATHbits.LATH0;
}

void toggle_tricolour_LED(void)
{
    //colour click mounted on buggy
        //colour click mounted on mikroBUS1 on buggy
            //set up initial LAT values
            LATGbits.LATG1 = !LATGbits.LATG1;                 //red LED pin
            LATAbits.LATA4 = !LATAbits.LATA4;                 //green LED pin
            LATFbits.LATF7 = !LATFbits.LATF7;                 //blue LED pin

        //colour click mounted on mikroBUS2 on buggy
            //set up initial LAT values
            LATGbits.LATG0 = !LATGbits.LATG0;                 //red LED pin
            LATEbits.LATE7 = !LATEbits.LATE7;                 //green LED pin
            LATAbits.LATA3 = !LATAbits.LATA3;                 //blue LED pin
    
    /*
    //colour click mounted on clicker board
        //colour click mounted on mikroBUS1 on clicker board
            //set up initial LAT values
            LATAbits.LATA0 = !LATAbits.LATA0;                 //red LED pin
            LATCbits.LATC2 = !LATCbits.LATC2;                 //green LED pin
            LATDbits.LATD0 = !LATDbits.LATD0;                 //blue LED pin

        //colour click mounted on mikroBUS2 on clicker board
            //set up initial LAT values
            LATAbits.LATA1 = !LATAbits.LATA1;                 //red LED pin
            LATCbits.LATC6 = !LATCbits.LATC6;                 //green LED pin
            LATHbits.LATH2 = !LATHbits.LATH2;                 //blue LED pin
    */
}