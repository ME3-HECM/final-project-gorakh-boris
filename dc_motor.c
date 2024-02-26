#include <xc.h>
#include "dc_motor.h"

// function initialise T2 and CCP for DC motor control
void initDCmotorsPWM(unsigned int PWMperiod){
    //initialise your TRIS and LAT registers for PWM  
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE4 = 0;
    TRISCbits.TRISC7 = 0;
    TRISGbits.TRISG6 = 0;
    
    LATEbits.LATE2 = 0;
    LATEbits.LATE4 = 0;
    LATCbits.LATC7 = 0;
    LATGbits.LATG6 = 0;
    
    //configure PPS to map CCP modules to pins
    RE2PPS=0x05; //CCP1 on RE2
    RE4PPS=0x06; //CCP2 on RE4
    RC7PPS=0x07; //CCP3 on RC7
    RG6PPS=0x08; //CCP4 on RG6

    // timer 2 config
    T2CONbits.CKPS=0b100; // 1:16 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    // 0.0001s*16MHz/16 -1 = 99
    T2PR=PWMperiod; //Period reg 10kHz base period
    T2CONbits.ON=1;
    
    //setup CCP modules to output PMW signals
    //initial duty cycles 
    CCPR1H=0; 
    CCPR2H=0; 
    CCPR3H=0; 
    CCPR4H=0; 
    
    //use tmr2 for all CCP modules used
    CCPTMRS0bits.C1TSEL=0;
    CCPTMRS0bits.C2TSEL=0;
    CCPTMRS0bits.C3TSEL=0;
    CCPTMRS0bits.C4TSEL=0;
    
    //configure each CCP
    CCP1CONbits.FMT=1; // left aligned duty cycle (we can just use high byte)
    CCP1CONbits.CCP1MODE=0b1100; //PWM mode  
    CCP1CONbits.EN=1; //turn on
    
    CCP2CONbits.FMT=1; // left aligned
    CCP2CONbits.CCP2MODE=0b1100; //PWM mode  
    CCP2CONbits.EN=1; //turn on
    
    CCP3CONbits.FMT=1; // left aligned
    CCP3CONbits.CCP3MODE=0b1100; //PWM mode  
    CCP3CONbits.EN=1; //turn on
    
    CCP4CONbits.FMT=1; // left aligned
    CCP4CONbits.CCP4MODE=0b1100; //PWM mode  
    CCP4CONbits.EN=1; //turn on
}

// function to set CCP PWM output from the values in the motor structure
void setMotorPWM(DC_motor *m)
{
    unsigned char posDuty, negDuty; //duty cycle values for different sides of the motor
    
    if(m->brakemode) {
        posDuty=m->PWMperiod - ((unsigned int)(m->power)*(m->PWMperiod))/100; //inverted PWM duty
        negDuty=m->PWMperiod; //other side of motor is high all the time
    }
    else {
        posDuty=0; //other side of motor is low all the time
		negDuty=((unsigned int)(m->power)*(m->PWMperiod))/100; // PWM duty
    }
    
    if (m->direction) {
        *(m->posDutyHighByte)=posDuty;  //assign values to the CCP duty cycle registers
        *(m->negDutyHighByte)=negDuty;       
    } else {
        *(m->posDutyHighByte)=negDuty;  //do it the other way around to change direction
        *(m->negDutyHighByte)=posDuty;
    }
}

//function to stop the robot gradually 
void stop(DC_motor *mL, DC_motor *mR)
{
    //mL->brakemode = 1;                  //set brake mode, probably not needed but doesn't hurt (so far)
    //mR->brakemode = 1;                  //set brake mode, probably not needed but doesn't hurt (so far)
    while ((mL->power>0) || (mR->power>0)){
        if (mL->power>0) {mL->power--;}
        if (mR->power>0) {mR->power--;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(rampDelay);
    }
}

//function to make the robot turn left 
void turnLeft(DC_motor *mL, DC_motor *mR)
{
    unsigned char leftGear = turningGear; 
    unsigned char rightGear = turningGear; 
    (mL->direction) = 0;                //set backward direction
    (mR->direction) = 1;                //set forward direction
    while ((mL->power<leftGear) || (mR->power<rightGear)){ //
        if (mL->power<leftGear) {mL->power++;}
        if (mR->power<rightGear) {mR->power++;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(rampDelay);
    }
}

//function to make the robot turn right 
void turnRight(DC_motor *mL, DC_motor *mR)
{
    unsigned char leftGear = turningGear; 
    unsigned char rightGear = turningGear; 
    (mL->direction) = 1;                //set forward direction
    (mR->direction) = 0;                //set backward direction
    while ((mL->power<leftGear) || (mR->power<rightGear)){ //
        if (mL->power<leftGear) {mL->power++;}
        if (mR->power<rightGear) {mR->power++;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(rampDelay);
    }
}

//function to make the robot go straight
void fullSpeedAhead(DC_motor *mL, DC_motor *mR)
{    
    unsigned char leftGear = topGear; 
    unsigned char rightGear = topGear; 
    
    if (topAdjustSide) {
        rightGear += topAdjustPower;
    } else {
        leftGear += topAdjustPower;
    }
    
    (mL -> direction) = 1;              //set forward direction
    (mR -> direction) = 1;              //set forward direction
    while ((mL->power<leftGear) || (mR->power<rightGear)){ //
        if (mL->power<leftGear) {mL->power++;}
        if (mR->power<rightGear) {mR->power++;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(rampDelay);
    }
}

void forward1(DC_motor *mL, DC_motor *mR)
{
    fullSpeedAhead(mL, mR);
    __delay_ms(forward1Delay);  //adjust until 1 metre
    stop(mL, mR);

}

void turnLeft90(DC_motor *mL, DC_motor *mR)
{
    turnLeft(mL, mR);
    __delay_ms(turnLeft90Delay);   //adjust until 90 degrees
    stop(mL, mR);
}

void turnRight90(DC_motor *mL, DC_motor *mR)
{
    turnRight(mL, mR);
    __delay_ms(turnRight90Delay);   //adjust until 90 degrees
    stop(mL, mR);
}

void turnLeft135(DC_motor *mL, DC_motor *mR)
{
    turnLeft(mL, mR);
    __delay_ms(turnLeft135Delay);   //adjust until 135 degrees
    stop(mL, mR);
}

void turnRight135(DC_motor *mL, DC_motor *mR)
{
    turnRight(mL, mR);
    __delay_ms(turnRight135Delay);  //adjust until 135 degrees
    stop(mL, mR);
}

void UTurn(DC_motor *mL, DC_motor *mR)
{
    turnLeft(mL, mR);
    __delay_ms(turn180Delay);   //adjust until 180 degrees
    stop(mL, mR);
}