#include <xc.h>
#include "interrupts.h"
#include "serial.h"
#include "timers.h"

void Interrupts_init(void)
{
    
    PIE4bits.RC4IE = 1; //recieve interrupt pin initialised (interrupt for serial communcation recieve)
    PIE4bits.TX4IE = 1; //transmit interrupt pin intialised (interrupt for serial communication send)
    
    PIE0bits.TMR0IE = 1;    //initialise timer 0 interrupt
    INTCONbits.PEIE = 1;    //initialise peripheral interrupt
    INTCONbits.GIE = 1;     //sets global interrupt
}



void __interrupt(high_priority) HighISR()
{    
    // timer interrupt
    if (PIR0bits.TMR0IF) {
        if (backtrack) {            //is backtracking
            //some code on performing the next manoeuvre when backtracking
            //the timer will be used to time the straight distances travelled when backtracking
        } else {                    //is not backtracking
            //trigger a lost function
        }
        //LATHbits.LATH3 = !LATHbits.LATH3;       //toggle LED for debugging
        PIR0bits.TMR0IF = 0;        //reset timer interrupt flag
    }
    
    
    //Serial Communication interrupt
    if (PIR4bits.TX4IF) {                           //transmit interrupt raised indicated that the transmit register is empty
        TX4REG = getCharFromTxBuf();                //transmit register gets the value from the buffer
        if (!isDataInTxBuf()) {PIE4bits.TX4IE = 0;} //this ensures all the bits from the buffer is sent
    }
    
    if (PIR4bits.RC4IF) {putCharToRxBuf(RC4REG);}  //if the receive interrupt is raised, the value is put into the review buffer
}
