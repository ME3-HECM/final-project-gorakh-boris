#include <xc.h>
#include "timers.h"

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON0bits.T016BIT=1;	// 16bit mode
    
    T0CON1bits.T0CKPS=0b1000;   //prescaler 1:256
    TMR0H=0b00001011;           //initial value of 3036
    TMR0L=0b11011100;           //overflow frequency 1Hz

    /*************************** (copied from mini project)
    switch (TESTING) {          //check for normal or testing operation
        case 0:                     //normal operation
            T0CON1bits.T0CKPS=0b1000;   //prescaler 1:256
            TMR0H=0b00001011;           //initial value of 3036
            TMR0L=0b11011100;           //overflow frequency 1Hz
            break;
        
        case 1:                     //testing hours operation
            T0CON1bits.T0CKPS=0b101;    //prescaler 1:32
            TMR0H=0b11111111;           //initial value of 65397
            TMR0L=0b01110101;           //overflow frequency 3597.1Hz
            break;
            
        case 2:                     //testing days operation
            T0CON1bits.T0CKPS=0b0;      //prescaler 1:1
            TMR0H=0b11111111;           //initial value of 65351
            TMR0L=0b01000111;           //overflow frequency 86486.5Hz
            break;
    }
    ***************************/
    
    T0CON0bits.T0EN=1;      //start the timer
    PIE0bits.TMR0IE = 1;    //initialise timer 0 interrupt
    INTCONbits.PEIE = 1;    //initialise peripheral interrupt
    INTCONbits.GIE = 1;     //sets global interrupt
}

void __interrupt() ISR()
{    
    // timer interrupt
    if (PIR0bits.TMR0IF) {
        /*************************** (copied from mini project)
        switch (TESTING) {          //check for normal or testing operation
            case 0:                     //normal operation
                TMR0H=0b00001011;           //initial value of 3036
                TMR0L=0b11011100;           //overflow frequency 1Hz
                break;

            case 1:                     //testing hours operation
                TMR0H=0b11111111;           //initial value of 65397
                TMR0L=0b01110101;           //overflow frequency 3597.1Hz
                break;

            case 2:                     //testing days operation
                TMR0H=0b11111111;           //initial value of 65351
                TMR0L=0b01000111;           //overflow frequency 86486.5Hz
                break;
        }
        ***************************/
        timer_count++;
        TMR0H=0b00001011;           //initial value of 3036
        TMR0L=0b11011100;           //overflow frequency 1Hz
        PIR0bits.TMR0IF = 0;        //reset timer interrupt flag
    }
}