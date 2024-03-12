#include <xc.h>
#include "buggy_lights.h"

/*******************************************************************************
 * Function to set up pin outs to lights on the buggy
*******************************************************************************/
void buggy_lights_init(void)
{
    //initialise car lights
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
    
    //initialise the tricolour LED on the colour click
        //colour click mounted on mikroBUS2 on buggy
            //set up TRIS registers (0 for output)
            TRISGbits.TRISG1 = 0;               //red LED pin
            TRISAbits.TRISA4 = 0;               //green LED pin
            TRISFbits.TRISF7 = 0;               //blue LED pin
            //set up initial LAT values
            LATGbits.LATG1 = 0;                 //red LED pin
            LATAbits.LATA4 = 0;                 //green LED pin
            LATFbits.LATF7 = 0;                 //blue LED pin

        //colour click mounted on mikroBUS1 on buggy
            //set up TRIS registers (0 for output)
            TRISGbits.TRISG0 = 0;               //red LED pin
            TRISEbits.TRISE7 = 0;               //green LED pin
            TRISAbits.TRISA3 = 0;               //blue LED pin
            //set up initial LAT values
            LATGbits.LATG0 = 0;                 //red LED pin
            LATEbits.LATE7 = 0;                 //green LED pin
            LATAbits.LATA3 = 0;                 //blue LED pin
}

/*******************************************************************************
 * Function to toggle the brake lights
*******************************************************************************/
void toggle_brake_lights(void)
{
    //toggle the LAT value
    LATDbits.LATD4 = !LATDbits.LATD4;
}

/*******************************************************************************
 * Function to toggle the main beam
*******************************************************************************/
void toggle_main_beam(void)
{
    //toggle the LAT value
    LATDbits.LATD3 = !LATDbits.LATD3;
}

/*******************************************************************************
 * Function to toggle the left indicators
*******************************************************************************/
void toggle_left_indicators(void)
{
    //toggle the LAT value
    LATFbits.LATF0 = !LATFbits.LATF0;
}

/*******************************************************************************
 * Function to toggle the right indicators
*******************************************************************************/
void toggle_right_indicators(void)
{
    //toggle the LAT value
    LATHbits.LATH0 = !LATHbits.LATH0;
}

/*******************************************************************************
 * Function to toggle the tricolour LED on the colour click
*******************************************************************************/
void toggle_tricolour_LED(void)
{
    //toggle the LAT values
        //colour click mounted on mikroBUS2 on buggy
        LATGbits.LATG1 = !LATGbits.LATG1;                 //red LED pin
        LATAbits.LATA4 = !LATAbits.LATA4;                 //green LED pin
        LATFbits.LATF7 = !LATFbits.LATF7;                 //blue LED pin

        //colour click mounted on mikroBUS1 on buggy
        LATGbits.LATG0 = !LATGbits.LATG0;                 //red LED pin
        LATEbits.LATE7 = !LATEbits.LATE7;                 //green LED pin
        LATAbits.LATA3 = !LATAbits.LATA3;                 //blue LED pin
}