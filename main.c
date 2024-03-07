/*
 * File:   main.c
 * Author: boris, gorakh
 *
 * Created on 26 February 2024, 11:27
 */

// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor.h"
#include "buggy_lights.h"
#include "manoeuvres.h"
#include "color.h"
#include "serial.h"
#include "timers.h"
#include "calibration.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    struct RGBC_val measured_colour;
        measured_colour.R = 0;
        measured_colour.G = 0;
        measured_colour.B = 0;
        measured_colour.C = 0;
    
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
    
    initDCmotorsPWM(PWMcycle);
    buggy_lights_init();
    color_click_init();
    initUSART4();
    Timer0_init();
    
    //initialise the two LEDs on the clicker board
        //set up TRIS registers (0 for output)
        TRISDbits.TRISD7 = 0;
        TRISHbits.TRISH3 = 0;
        //set up initial LAT values
        LATDbits.LATD7 = 0;
        LATHbits.LATH3 = 0;
    
    //initialise the two push buttons on clicker board
        //set up TRIS registers (1 for input)
        TRISFbits.TRISF2 = 1;
        TRISFbits.TRISF3 = 1;
        //set up pin analogue/digital inputs (0 for digital)
        ANSELFbits.ANSELF2 = 0;
        ANSELFbits.ANSELF3 = 0;
    
    //test_manoeuvres(&motorL, &motorR, returning);
    
    //while (PORTFbits.RF3);              //wait until RF3 is pressed
    LATHbits.LATH3 = !LATHbits.LATH3;   //toggle RH3 LED for debugging
    toggle_tricolour_LED();
    
    //forward_navigation(&motorL, &motorR, &measured_colour);
    
    while (1) {
        getRGBCval(&measured_colour);
        getHSVval(&HSV_colour, &measured_colour);
        sendRGBCvalSerial4(&measured_colour);
        sendHSVvalSerial4(&HSV_colour);
        __delay_ms(1000);
    }
}
