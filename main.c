/*
 * File:   main.c
 * Author: boris, gorakh
 *
 * Created on 26 February 2024, 11:27
 */

// CONFIG1L
#pragma config FEXTOSC = HS         //External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL //Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31   //WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF           //WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor.h"
#include "buggy_lights.h"
#include "manoeuvres.h"
#include "color.h"
#include "serial.h"
#include "timers.h"
#include "calibration.h"

#define _XTAL_FREQ 64000000         //note intrinsic _delay function is 62.5ns at 64,000,000Hz
#define DRIVE 1                     //normal operation, not testing anything
#define COLOUR_TEST 0               //testing code and sending to serial
#define MOTOR_TEST 0                //calibrating motor forward and turns
#define TIMER_TEST 0                //testing timer and memory functions

void main(void) {
    struct RGBC_val RGBC_colour;
        RGBC_colour.R = 0;
        RGBC_colour.G = 0;
        RGBC_colour.B = 0;
        RGBC_colour.C = 0;
    
    struct HSV_val HSV_colour;
        HSV_colour.H = 0;
        HSV_colour.S = 0;
        HSV_colour.V = 0;
    
    unsigned int PWMcycle = 99;    
    struct DC_motor motorL, motorR;
        motorL.power = 0;
        motorL.direction = 1;
        motorL.brakemode = 1;
        motorL.PWMperiod = PWMcycle;
        motorL.posDutyHighByte = (unsigned char *)(&CCPR1H);
        motorL.negDutyHighByte = (unsigned char *)(&CCPR2H);
        motorR.power = 0;
        motorR.direction = 1;
        motorR.brakemode = 1;
        motorR.PWMperiod = PWMcycle;
        motorR.posDutyHighByte = (unsigned char *)(&CCPR3H);
        motorR.negDutyHighByte = (unsigned char *)(&CCPR4H);
    
    //initialise lots-a-functions
    initDCmotorsPWM(PWMcycle);
    buggy_lights_init();
    color_click_init();
    initUSART4();
    Timer0_init();
    
    /* initialise the two LEDs on the clicker board */
        //set up TRIS registers (0 for output)
        TRISDbits.TRISD7 = 0;
        TRISHbits.TRISH3 = 0;
        //set up initial LAT values
        LATDbits.LATD7 = 0;
        LATHbits.LATH3 = 0;
    
    /* initialise the two push buttons on clicker board */
        //set up TRIS registers (1 for input)
        TRISFbits.TRISF2 = 1;
        TRISFbits.TRISF3 = 1;
        //set up pin analogue/digital inputs (0 for digital)
        ANSELFbits.ANSELF2 = 0;
        ANSELFbits.ANSELF3 = 0;
    
    //wait until RF3 button is pressed
    while (PORTFbits.RF3);
    
    //toggle both LEDS
    LATDbits.LATD7 = !LATDbits.LATD7;
    LATHbits.LATH3 = !LATHbits.LATH3;
    
    //wait a sec
    __delay_ms(1000);
    
#if DRIVE
    /***********************************************************************
     * Go forward in the maze, function loops until buggy has reached the
     * end or is lost due to an exception
    ***********************************************************************/
    forward_navigation(&motorL, &motorR, &HSV_colour, &RGBC_colour);
    
    /***********************************************************************
     * Return to the start of the maze, function loops until buggy has
     * performed all manoeuvres stored in memory (where it hopefully has
     * reached the end)
     * 
     * RH3 LED turns off if buggy is returning without being lost
     * Both RD7 and RH3 LEDs turn off if buggy is lost and returning
    ***********************************************************************/
    return_to_sender(&motorL, &motorR);
#endif
    
#if COLOUR_TEST   
    while (1) {
        average_RGBC(&RGBC_colour);
        scale_RGB(&RGBC_colour);
        convert_HSV(&HSV_colour, &RGBC_colour);
        
        unsigned char k = colour_to_key(&HSV_colour, &RGBC_colour);
        
        sendRGBCvalSerial4(&RGBC_colour);
        sendHSVvalSerial4(&HSV_colour);
        sendIntSerial4((int)k);
        
        __delay_ms(500);
    }
#endif
    
#if MOTOR_TEST
    //pick_card(&motorL, &motorR, returning, 1);        //red
    //pick_card(&motorL, &motorR, returning, 2);        //green
    //pick_card(&motorL, &motorR, returning, 3);        //blue
    //pick_card(&motorL, &motorR, returning, 4);        //yellow
    //pick_card(&motorL, &motorR, returning, 5);        //pink
    //pick_card(&motorL, &motorR, returning, 6);        //orange
    //pick_card(&motorL, &motorR, returning, 7);        //cyan
    //pick_card(&motorL, &motorR, returning, 8);        //white
    
    //headbuttReverse(&motorL, &motorR);
    //squareReverse(&motorL, &motorR);
    
    //while (1) {fullSpeedAhead(&motorL, &motorR);}
    
    for (unsigned char i = 0; i < 4; i++) {
        //turnLeft90(&motorL, &motorR);
        //turnRight90(&motorL, &motorR);
        //turnLeft135(&motorL, &motorR);
        //turnRight135(&motorL, &motorR);
        UTurn(&motorL, &motorR);
        //break;
    }
#endif

#if TIMER_TEST
    //temporary timer and manoeuvre variables
    unsigned char timerH = 0;
    unsigned char timerL = 0;
    unsigned char mann = 0;

    //set timer to zero, start timer and drive forward
    reset_timer();
    start_timer();
    fullSpeedAhead(&motorL, &motorR);

    //wait a while
    __delay_ms(2000);

    //stop timer, read the timer registers and stop driving
    stop_timer();
    read_timer(&timerH, &timerL);
    stop(&motorL, &motorR);
    
    //send to serial for debugging
    sendIntSerial4(timerH);
    sendIntSerial4(timerL);
    
    //write variables to memory
    write_trail(timerH, timerL, mann);
    
    ////////////////////////////////////////////////////////////////////////
    returning = 1;
    //manually rotate buggy 180 degrees
    while (PORTFbits.RF3);
    __delay_ms(1000);
    ////////////////////////////////////////////////////////////////////////
    
    //read variables from memory
    read_trail(&timerH, &timerL, &mann);
    
    sendIntSerial4(timerH);                 //send to serial for debugging
    sendIntSerial4(timerL);                 //send to serial for debugging
    
    //write timer, start timer and drive forwards
    write_timer(0b11111111 - timerH, 0b11111111 - timerL);
    start_timer();
    fullSpeedAhead(&motorL, &motorR);

    //wait until timer overflow raises return flag
    while (!return_flag);

    //stop driving, stop timer and lower return flag
    stop(&motorL, &motorR);
    stop_timer();
    return_flag = 0;
#endif
}
