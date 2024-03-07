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
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    __delay_ms(1000);    //initial delay
    
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
    

    //CREATING A VARIABLE FOR THE STRUCTURE RGBC_VAL
    struct RGBC_val readRGBC;
    
    readRGBC.R = 0;
    readRGBC.B = 0;
    readRGBC.G = 0;
    readRGBC.C = 0;
    
    
    struct HSV_val readHSV;
    
    readHSV.H = 0;
    readHSV.S = 0;
    readHSV.V = 0;
    
    
    //INITIALISATION
    buggy_lights_init();
    color_click_init();
    initUSART4();
    Timer0_init();
    Interrupts_init();
    
    
    //initialise the two push buttons on clicker board
    //set up TRIS registers (1 for input)
    TRISFbits.TRISF2 = 1;
    TRISFbits.TRISF3 = 1;
    //set up pin analogue/digital inputs (0 for digital)
    ANSELFbits.ANSELF2 = 0;
    ANSELFbits.ANSELF3 = 0;
    
    //fullSpeedAhead(&motorL, &motorR);
    //cardRed(&motorL, &motorR, backtrack);
    //cardGreen(&motorL, &motorR, backtrack);
    //cardBlue(&motorL, &motorR, backtrack);
    //cardYellow(&motorL, &motorR, backtrack);
    //cardPink(&motorL, &motorR, backtrack);
    //cardOrange(&motorL, &motorR, backtrack);
    //cardCyan(&motorL, &motorR, backtrack);
    //cardWhite(&motorL, &motorR);
    
    
    char display[50];

 
    
    while (1) {
        //sendIntSerial4((int)TMR0L);
        //sendI ntSerial4((int)TMR0H);
        //sendIntSerial4(sizeof(trail_manoeuvre));
        if (!PORTFbits.RF2) {  //on button press
            *manoeuvre_pointer = 0;
            manoeuvre_pointer ++;
            LATDbits.LATD7 = !LATDbits.LATD7;
        }
        getRGBCval(&readRGBC);
        getHSVval(&readHSV,&readRGBC);
        sprintf(display, "%d %d %d %d %d %d %d \r",readRGBC.R,readRGBC.G,readRGBC.B,readRGBC.C,readHSV.H,readHSV.S,readHSV.V);
        TxBufferedString(display);
        sendTxBuf();
        
        //sendArraySerial4(trail_manoeuvre);
        __delay_ms(500);
    }
}
