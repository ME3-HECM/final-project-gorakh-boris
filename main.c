/*
 * File:   main.c
 * Author: boris
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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    unsigned int PWMcycle = 99;
    initDCmotorsPWM(PWMcycle);
    
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
    
    buggy_lights_init();
    
    while (1) {
        //fullSpeedAhead(&motorL, &motorR);
        LATDbits.LATD4 = !LATDbits.LATD4;       //flash brake lights (DOESN'T WORK)
        LATDbits.LATD3 = !LATDbits.LATD3;       //flash main beam
        __delay_ms(500);
        LATFbits.LATF0 = !LATFbits.LATF0;       //flash left indicators
        LATHbits.LATH0 = !LATHbits.LATH0;       //flash right indicators
    }
}
