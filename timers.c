#include <xc.h>
#include "timers.h"

/*******************************************************************************
 * Function to set up timer 0
*******************************************************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS = 0b010;        //Fosc/4
    T0CON1bits.T0ASYNC = 1;         //see datasheet errata - needed to ensure 
                                    //correct operation when Fosc/4 used 
                                    //as clock source
    
    T0CON0bits.T016BIT = 1;         //16-bit mode
    T0CON1bits.T0CKPS = 0b1110;     //prescaler 1:16384
                                    //1.024 millisecond timer increment period
                                    //976.5625 increments per second
                                    //67.108864 seconds before overflow
    
    reset_timer();                  //set timer register to zero
    
    PIE0bits.TMR0IE = 1;            //initialise timer0 interrupt
    INTCONbits.PEIE = 1;            //initialise peripheral interrupt
    INTCONbits.GIE = 1;             //sets global interrupt
}

/*******************************************************************************
 * Function to start the timer
*******************************************************************************/
void start_timer(void)
{
    T0CON0bits.T0EN = 1;            //start the timer
}

/*******************************************************************************
 * Function to stop the timer
*******************************************************************************/
void stop_timer(void)
{
    T0CON0bits.T0EN = 0;            //stop the timer
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
    //loop while not returning to start
    while (!returning) {
        
        //temporary timer and manoeuvre variables
        unsigned char timerH = 0;
        unsigned char timerL = 0;
        unsigned char mann = 0;
        
        //set timer to zero, start timer and drive forward
        reset_timer();
        start_timer();
        fullSpeedAhead(mL, mR);
        
        //wait until a wall is detected or lost flag
        wait_for_wall(p2, lost_flag);
        
        //stop timer, read the timer registers and stop driving
        stop_timer();
        read_timer(&timerH, &timerL);
        stop(mL, mR);
        
        //turn on tricolour LED and wait a moment
        toggle_tricolour_LED();
        __delay_ms(200);
        
        //sample RGBC, apply scaling, convert to HSV and obtain a manoeuvre key
        average_RGBC(p2);
        scale_RGB(p2);
        convert_HSV(p1, p2);
        mann = colour_to_key(p1, p2);
        
        //turn off tricolour LED and wait a moment
        toggle_tricolour_LED();
        __delay_ms(200);
        
        //if lost flag raised from timer overflow
        if (lost_flag) {
            
            //write maximum 16-bit value to timer variables
            timerH = 0b11111111;
            timerL = 0b11111111;
            
            //white card manoeuvre allows buggy to U turn
            mann = 8;
            
            //toggle LED corresponding to lost condition
            LATDbits.LATD7 = !LATDbits.LATD7;
        }
        
        //if lost condition triggered due to memory end reached or black card
        if (manoeuvre_count == 19 || mann == 9) {
            
            //white card manoeuvre allows buggy to U turn
            mann = 8;
            
            //toggle LED corresponding to lost condition
            LATDbits.LATD7 = !LATDbits.LATD7;
        }
        
        //write temporary variables to memory
        write_trail(timerH, timerL, mann);
        
        //perform manoeuvre based on manoeuvre key
        pick_card(mL, mR, returning, mann);
        
        //if white card detected or lost condition triggered
        if (mann == 8) {
            
            //trigger returning condition
            returning = 1;
            
            //toggle LED corresponding to returning condition
            LATHbits.LATH3 = !LATHbits.LATH3;
        }
        
#if SERIAL_FEEDBACK
        sendRGBCvalSerial4(p2);
        sendHSVvalSerial4(p1);
        sendArrayCharSerial4(trail_timer_high);
        sendArrayCharSerial4(trail_timer_low);
        sendArrayCharSerial4(trail_manoeuvre);
#endif
    }
}

/*******************************************************************************
 * Integrated function to navigate back to the start of the maze
*******************************************************************************/
void return_to_sender(DC_motor *mL, DC_motor *mR)
{
    //loop until all manoeuvres performed
    while (manoeuvre_count != 0) {
        
        //temporary timer and manoeuvre variables
        unsigned char timerH = 0;
        unsigned char timerL = 0;
        unsigned char mann = 0;
        
        //read variables from memory
        read_trail(&timerH, &timerL, &mann);
        
        //perform manoeuvre but ignore white card instruction
        if (mann != 8) {
            pick_card(mL, mR, returning, mann);
        }
        
        //toggle main beam for road safety
        toggle_main_beam();
        
        //write timer, start timer and drive forwards
        write_timer(0b11111111 - timerH, 0b11111111 - timerL);
        start_timer();
        fullSpeedAhead(mL, mR);
        
        //wait until timer overflow raises return flag
        while (!return_flag);
        
        //stop driving, stop timer and lower return flag
        stop(mL, mR);
        stop_timer();
        return_flag = 0;
        
        //toggle main beam for road safety
        toggle_main_beam();
        
#if SERIAL_FEEDBACK
        sendIntSerial4(timerH);                 //send to serial for debugging
        sendIntSerial4(timerL);                 //send to serial for debugging
        sendIntSerial4(mann);                   //send to serial for debugging
#endif
    }
}

/*******************************************************************************
 * Interrupt service routine for use with the timer overflow interrupt source
*******************************************************************************/
void __interrupt() ISR()
{
    //when timer interrupt flag is raised
    if (PIR0bits.TMR0IF) {
        if (returning) {
            //raise return flag if buggy is returning to start
            return_flag = 1;
        } else {
            //raise lost flag is buggy is not returning to start
            lost_flag = 1;
        }
        
        //lower timer interrupt flag
        PIR0bits.TMR0IF = 0;
    }
}