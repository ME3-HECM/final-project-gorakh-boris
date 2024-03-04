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
    
    T0CON1bits.T0CKPS=0b1110;   //prescaler 1:16384
                                //1.024 millisecond timer increment period
                                //976.5625 increments per second
                                //67.108864 seconds before overflow
    //REMEMBER TIMER READ AND WRITE ORDER
    //Read TMR0L first before TMR0H
    //Write TMR0H first before TMR0L
    TMR0H = 0;                  //initial zero value
    TMR0L = 0;                  //initial zero value
    
    T0CON0bits.T0EN=1;      //start the timer
    PIE0bits.TMR0IE = 1;    //initialise timer 0 interrupt
    INTCONbits.PEIE = 1;    //initialise peripheral interrupt
    INTCONbits.GIE = 1;     //sets global interrupt
}

void writeTrail(unsigned char *man) {
    *timer_high_pointer = TMR0H;    
    *timer_low_pointer = TMR0L;    
    *manoeuvre_pointer = *man;
    
    timer_high_pointer ++;
    timer_low_pointer ++;
    manoeuvre_pointer ++;
    
    manoeuvre_count ++;
}

void readTrail(unsigned char *tH, unsigned char *tL, unsigned char *man) {
    timer_high_pointer --;
    timer_low_pointer --;
    manoeuvre_pointer --;
    
    *tH = *timer_high_pointer;
    *tL = *timer_low_pointer;
    *man = *manoeuvre_pointer;
    
    manoeuvre_count --;
}

void returnToSender(DC_motor *mL, DC_motor *mR) {
    while (manoeuvre_count != 0) {
        unsigned char timerH = 0;
        unsigned char timerL = 0;
        unsigned char mann = 0;
        readTrail(&timerH, &timerL, &mann);
        //sendIntSerial4(timerH);
        //sendIntSerial4(timerL);
        //sendIntSerial4(mann);
        if (mann != 8) {            //ignore white card instruction
            pickCard(mL, mR, returning, mann);
        }
        TMR0H = 0b11111111 - timerH;
        TMR0L = 0b11111111 - timerL;
        fullSpeedAhead(mL, mR);
        while (!returnFlag);
        stop(mL, mR);
        returnFlag = 0;
    }
    LATHbits.LATH3 = !LATHbits.LATH3;       //toggle LED for debugging
}

void __interrupt() ISR()
{    
    // timer interrupt
    if (PIR0bits.TMR0IF) {
        if (returning) {            //is backtracking
            //some code on performing the next manoeuvre when backtracking
            //the timer will be used to time the straight distances travelled when backtracking
            returnFlag = 1;
        } else {                    //is not backtracking
            //trigger a lost function
        }
        //LATHbits.LATH3 = !LATHbits.LATH3;       //toggle LED for debugging
        PIR0bits.TMR0IF = 0;        //reset timer interrupt flag
    }
}