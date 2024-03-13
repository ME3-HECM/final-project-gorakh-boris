#include <xc.h>
#include "timers.h"

/*******************************************************************************
 * Function to set up timer 0
*******************************************************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010;      //Fosc/4
    T0CON1bits.T0ASYNC=1;       //see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON0bits.T016BIT=1;       //16-bit mode
    
    T0CON1bits.T0CKPS=0b1110;   //prescaler 1:16384
                                //1.024 millisecond timer increment period
                                //976.5625 increments per second
                                //67.108864 seconds before overflow
    
    reset_timer();              //set timer register to zero
    T0CON0bits.T0EN=1;          //start the timer
    
    start_timer();
    PIE0bits.TMR0IE = 1;        //initialise timer0 interrupt
    INTCONbits.PEIE = 1;        //initialise peripheral interrupt
    INTCONbits.GIE = 1;         //sets global interrupt
}

/*******************************************************************************
 * Function to start the timer
*******************************************************************************/
void start_timer(void)
{
    T0CON0bits.T0EN=1;          //start the timer
}

/*******************************************************************************
 * Function to stop the timer
*******************************************************************************/
void stop_timer(void)
{
    T0CON0bits.T0EN=0;          //stop the timer
}

/*******************************************************************************
 * Function to read timer register and store in timer high and low variables
*******************************************************************************/
void read_timer(unsigned char *tH, unsigned char *tL)
{
    //remember timer read and write order
    //read TMR0L first before TMR0H
    //write TMR0H first before TMR0L
    *tL = TMR0L;
    *tH = TMR0H;
}

/*******************************************************************************
 * Function to write timer register from timer high and low variables
*******************************************************************************/
void write_timer(unsigned char tH, unsigned char tL)
{
    //remember timer read and write order
    //read TMR0L first before TMR0H
    //write TMR0H first before TMR0L
    TMR0H = tH;
    TMR0L = tL;
}

/*******************************************************************************
 * Function to reset the timer register to zero
*******************************************************************************/
void reset_timer(void)
{
    write_timer(0, 0);
}

/*******************************************************************************
 * Function to read from the stored trail memory
 * A set of timer high and low values
 * A manoeuvre key value
 * Pointers and counters are decremented BEFORE reading memory
*******************************************************************************/
void read_trail(unsigned char *tH, unsigned char *tL, unsigned char *man)
{
    timer_high_pointer --;              //decrement timer high memory pointer
    timer_low_pointer --;               //decrement timer low memory pointer
    manoeuvre_pointer --;               //decrement manoeuvre memory pointer
    
    manoeuvre_count --;                 //decrement manoeuvre counter
    
    *tH = *timer_high_pointer;          //read from timer high memory
    *tL = *timer_low_pointer;           //read from timer low memory
    *man = *manoeuvre_pointer;          //read from manoeuvre memory
}

/*******************************************************************************
 * Function to write to the stored trail memory
 * A set of timer high and low values
 * A manoeuvre key value
 * Pointers and counters are incremented AFTER writing memory
*******************************************************************************/
void write_trail(unsigned char tH, unsigned char tL, unsigned char man)
{
    *timer_high_pointer = tH;           //write to timer high memory
    *timer_low_pointer = tL;            //write to timer low memory
    *manoeuvre_pointer = man;           //write to manoeuvre memory
    
    timer_high_pointer ++;              //increment timer high memory pointer
    timer_low_pointer ++;               //increment timer low memory pointer
    manoeuvre_pointer ++;               //increment manoeuvre memory pointer
    
    manoeuvre_count ++;                 //increment manoeuvre counter
}

/*******************************************************************************
 * Integrated function for forward navigation in maze
*******************************************************************************/
void forward_navigation(DC_motor *mL, DC_motor *mR, HSV_val *p1, RGBC_val *p2)
{
    while (!returning) {                //loop while not returning to start
        unsigned char timerH = 0;       //temporary timer high variable
        unsigned char timerL = 0;       //temporary timer low variable
        unsigned char mann = 0;         //temporary manoeuvre variable
        
        reset_timer();
        start_timer();
        fullSpeedAhead(mL, mR);         //go forward continuously
        
        wait_for_wall(p2, lost_flag);   //wait until wall detected or lost flag
        
        read_timer(&timerH, &timerL);
        stop_timer();
        stop(mL, mR);                   //stop motors
        
        toggle_tricolour_LED();         //turn on tricolour LED
        __delay_ms(200);
        
        average_RGBC(p2);
        scale_RGB(p2);

        convert_HSV(p1, p2);
        mann = colour_to_key(p1, p2);
        
        toggle_tricolour_LED();         //turn off tricolour LED
        __delay_ms(200);
        
        /***************************************************************
         * By the time the read_timer above is performed, the timer
         * registers will have already overflowed
         * 
         * The if-statement below writes the maximum 8-bit values to
         * the temporary variables instead
        ***************************************************************/
        if (lost_flag) {                    //if lost
            timerH = 0b11111111;
            timerL = 0b11111111;
            mann = 8;
            LATDbits.LATD7 = !LATDbits.LATD7;
        }
        
        if (manoeuvre_count == 19) {        //when counter is at the last memory
            mann = 8;
        }
        
        write_trail(timerH, timerL, mann);          //write variables to memory
        pick_card(mL, mR, returning, mann);         //perform manoeuvre
        
        if (mann == 8) {                            
            returning = 1;
            LATHbits.LATH3 = !LATHbits.LATH3;
        }
        
        sendRGBCvalSerial4(p2);
        sendHSVvalSerial4(p1);
        sendArrayCharSerial4(trail_timer_high);
        sendArrayCharSerial4(trail_timer_low);
        sendArrayCharSerial4(trail_manoeuvre);
    }
}

/*******************************************************************************
 * Integrated function to navigate back to the start of the maze
*******************************************************************************/
void return_to_sender(DC_motor *mL, DC_motor *mR)
{
    while (manoeuvre_count != 0) {      //loop until all manoeuvres performed
        unsigned char timerH = 0;       //temporary timer high variable
        unsigned char timerL = 0;       //temporary timer low variable
        unsigned char mann = 0;         //temporary manoeuvre variable
        
        read_trail(&timerH, &timerL, &mann);    //read variables from memory
        sendIntSerial4(timerH);                 //send to serial for debugging
        sendIntSerial4(timerL);                 //send to serial for debugging
        sendIntSerial4(mann);                   //send to serial for debugging
        if (mann != 8) {                        //ignore white card instruction
            pick_card(mL, mR, returning, mann); //perform manoeuvre
        }
        
        toggle_main_beam();
        write_timer(0b11111111 - timerH, 0b11111111 - timerL);
        start_timer();
        fullSpeedAhead(mL, mR);         //go forward continuously
        
        while (!return_flag);           //wait until timer overflows
        
        stop(mL, mR);                   //stop motors
        stop_timer();
        return_flag = 0;                //reset return flag
        toggle_main_beam();
    }
}

/*******************************************************************************
 * Interrupt service routine for use with the timer overflow interrupt source
*******************************************************************************/
void __interrupt() ISR()
{
    if (PIR0bits.TMR0IF) {              //when timer interrupt flag is raised
        if (returning) {                //is backtracking
            return_flag = 1;            //raise return flag
        } else {                        //is not backtracking
            lost_flag = 1;              //raise lost flag
        }
        PIR0bits.TMR0IF = 0;            //reset timer interrupt flag
    }
}