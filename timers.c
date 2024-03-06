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
    
    reset_timer();
    T0CON0bits.T0EN=1;      //start the timer
    
    PIE0bits.TMR0IE = 1;    //initialise timer 0 interrupt
    INTCONbits.PEIE = 1;    //initialise peripheral interrupt
    INTCONbits.GIE = 1;     //sets global interrupt
}

void read_timer(unsigned char *tH, unsigned char *tL)
{
    //remember timer read and write order
    //read TMR0L first before TMR0H
    //write TMR0H first before TMR0L
    *tL = TMR0L;
    *tH = TMR0H;
}

void write_timer(unsigned char tH, unsigned char tL)
{
    //remember timer read and write order
    //read TMR0L first before TMR0H
    //write TMR0H first before TMR0L
    TMR0H = tH;
    TMR0L = tL;
}

void reset_timer(void)
{
    write_timer(0, 0);
}

void write_trail(unsigned char *man)
{
    *timer_high_pointer = TMR0H;    
    *timer_low_pointer = TMR0L;    
    *manoeuvre_pointer = *man;
    
    timer_high_pointer ++;
    timer_low_pointer ++;
    manoeuvre_pointer ++;
    
    manoeuvre_count ++;
}

void read_trail(unsigned char *tH, unsigned char *tL, unsigned char *man)
{
    timer_high_pointer --;
    timer_low_pointer --;
    manoeuvre_pointer --;
    
    *tH = *timer_high_pointer;
    *tL = *timer_low_pointer;
    *man = *manoeuvre_pointer;
    
    manoeuvre_count --;
}

void return_to_sender(DC_motor *mL, DC_motor *mR)
{
    while (manoeuvre_count != 0) {
        unsigned char timerH = 0;
        unsigned char timerL = 0;
        unsigned char mann = 0;
        read_trail(&timerH, &timerL, &mann);
        //sendIntSerial4(timerH);
        //sendIntSerial4(timerL);
        //sendIntSerial4(mann);
        if (mann != 8) {            //ignore white card instruction
            pick_card(mL, mR, returning, mann);
        }
        write_timer(0b11111111 - timerH, 0b11111111 - timerL);
        fullSpeedAhead(mL, mR);
        while (!return_flag);
        stop(mL, mR);
        return_flag = 0;
    }
    //LATHbits.LATH3 = !LATHbits.LATH3;       //toggle LED for debugging
}

void __interrupt() ISR()
{    
    // timer interrupt
    if (PIR0bits.TMR0IF) {
        if (returning) {            //is backtracking
            return_flag = 1;
        } else {                    //is not backtracking
            //trigger a lost function
        }
        //LATHbits.LATH3 = !LATHbits.LATH3;       //toggle LED for debugging
        PIR0bits.TMR0IF = 0;        //reset timer interrupt flag
    }
}