#include <xc.h>
#include "dc_motor.h"

/*******************************************************************************
 * Function initialise T2 and CCP for DC motor control
*******************************************************************************/
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
    /***********************************************************************
     * Tpwm*(Fosc/4)/prescaler - 1 = PTPER
     * 0.0001s*16MHz/16 -1 = 99
    ***********************************************************************/
    T2PR=(unsigned char)PWMperiod; //Period reg 10kHz base period
    T2CONbits.ON=1;
    
    /* setup CCP modules to output PMW signals */
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

/*******************************************************************************
 * Function to set CCP PWM output from the values in the motor structure
*******************************************************************************/
void setMotorPWM(DC_motor *m)
{
    unsigned char posDuty, negDuty; //duty cycle values for different sides of the motor
    
    if(m->brakemode) {
        posDuty=(unsigned char)(m->PWMperiod - ((unsigned int)(m->power)*(m->PWMperiod))/100); //inverted PWM duty
        negDuty=(unsigned char)(m->PWMperiod); //other side of motor is high all the time
    }
    else {
        posDuty=0; //other side of motor is low all the time
		negDuty=(unsigned char)(((unsigned int)(m->power)*(m->PWMperiod))/100); // PWM duty
    }
    
    if (m->direction) {
        *(m->posDutyHighByte)=posDuty;  //assign values to the CCP duty cycle registers
        *(m->negDutyHighByte)=negDuty;       
    } else {
        *(m->posDutyHighByte)=negDuty;  //do it the other way around to change direction
        *(m->negDutyHighByte)=posDuty;
    }
}

/*******************************************************************************
 * Function to stop the robot
*******************************************************************************/
void stop(DC_motor *mL, DC_motor *mR)
{
    //loop until both motor powers are 0
    while ((mL->power > 0) || (mR->power > 0)){
        
        //only reduce left power if above 0
        if (mL->power > 0) {mL->power--;}
        
        //only reduce right power if above 0
        if (mR->power > 0) {mR->power--;}
        
        //set motor parameters
        setMotorPWM(mL);
        setMotorPWM(mR);
        
        //delay allows for smooth deceleration
        __delay_ms(rampDelay);
    }
}

/*******************************************************************************
 * Function to make the robot turn left continuously
*******************************************************************************/
void turnLeft(DC_motor *mL, DC_motor *mR)
{
    //assign motor power targets
    unsigned char leftGear = turningLeftGear;                   //left motor
    unsigned char rightGear = turningLeftGear;                  //right motor
    
    //assign motor directions
    (mL->direction) = 0;                                        //left motor
    (mR->direction) = 1;                                        //right motor
    
    //loop until both motor powers reach targets
    while ((mL->power < leftGear) || (mR->power < rightGear)){
        
        //only increase left power if less than target
        if (mL->power < leftGear) {mL->power++;}
        
        //only increase right power if less than target
        if (mR->power < rightGear) {mR->power++;}
        
        //set motor parameters
        setMotorPWM(mL);
        setMotorPWM(mR);
        
        //delay allows for smooth acceleration
        __delay_ms(rampDelay);
    }
}

/*******************************************************************************
 * Function to make the robot turn right continuously
*******************************************************************************/
void turnRight(DC_motor *mL, DC_motor *mR)
{
    //assign motor power targets
    unsigned char leftGear = turningRightGear;                  //left motor
    unsigned char rightGear = turningRightGear;                 //right motor
    
    //assign motor directions
    (mL->direction) = 1;                                        //left motor
    (mR->direction) = 0;                                        //right motor
    
    //loop until both motor powers reach targets
    while ((mL->power < leftGear) || (mR->power < rightGear)){
        
        //only increase left power if less than target
        if (mL->power < leftGear) {mL->power++;}
        
        //only increase right power if less than target
        if (mR->power < rightGear) {mR->power++;}
        
        //set motor parameters
        setMotorPWM(mL);
        setMotorPWM(mR);
        
        //delay allows for smooth acceleration
        __delay_ms(rampDelay);
    }
}

/*******************************************************************************
 * Function to make the robot go straight continuously
*******************************************************************************/
void fullSpeedAhead(DC_motor *mL, DC_motor *mR)
{
    //assign motor power targets
    unsigned char leftGear = topGearLeft;                       //left motor
    unsigned char rightGear = topGearRight;                     //right motor
    
    //assign motor directions
    (mL->direction) = 1;                                        //left motor
    (mR->direction) = 1;                                        //right motor
    
    //loop until both motor powers reach targets
    while ((mL->power < leftGear) || (mR->power < rightGear)){
        
        //only increase left power if less than target
        if (mL->power < leftGear) {mL->power++;}
        
        //only increase right power if less than target
        if (mR->power < rightGear) {mR->power++;}
        
        //set motor parameters
        setMotorPWM(mL);
        setMotorPWM(mR);
        
        //delay allows for smooth acceleration
        __delay_ms(rampDelay);
    }
}

/*******************************************************************************
 * Function to make the robot go reverse continuously
*******************************************************************************/
void fullSpeedReverse(DC_motor *mL, DC_motor *mR)
{
    //assign motor power targets
    unsigned char leftGear = bottomGearLeft;                    //left motor
    unsigned char rightGear = bottomGearRight;                  //right motor
    
    //assign motor directions
    (mL->direction) = 0;                                        //left motor
    (mR->direction) = 0;                                        //right motor
    
    //loop until both motor powers reach targets
    while ((mL->power < leftGear) || (mR->power < rightGear)){
        
        //only increase left power if less than target
        if (mL->power < leftGear) {mL->power++;}
        
        //only increase right power if less than target
        if (mR->power < rightGear) {mR->power++;}
        
        //set motor parameters
        setMotorPWM(mL);
        setMotorPWM(mR);
        
        //delay allows for smooth acceleration
        __delay_ms(rampDelay);
    }
}

/*******************************************************************************
 * Function to make the robot turn left for 90 degrees
*******************************************************************************/
void turnLeft90(DC_motor *mL, DC_motor *mR)
{
    toggle_left_indicators();           //indicate left for road safety
    turnLeft(mL, mR);                   //set motors to continuously turn left
    __delay_ms(turnLeft90Delay);        //adjust delay until 90 degrees
    stop(mL, mR);                       //stop motors
    toggle_left_indicators();           //indicate left for road safety
}

/*******************************************************************************
 * Function to make the robot turn right for 90 degrees
*******************************************************************************/
void turnRight90(DC_motor *mL, DC_motor *mR)
{
    toggle_right_indicators();          //indicate right for road safety
    turnRight(mL, mR);                  //set motors to continuously turn right
    __delay_ms(turnRight90Delay);       //adjust delay until 90 degrees
    stop(mL, mR);                       //stop motors
    toggle_right_indicators();          //indicate right for road safety
}

/*******************************************************************************
 * Function to make the robot turn left for 135 degrees
*******************************************************************************/
void turnLeft135(DC_motor *mL, DC_motor *mR)
{
    toggle_left_indicators();           //indicate left for road safety
    turnLeft(mL, mR);                   //set motors to continuously turn left
    __delay_ms(turnLeft135Delay);       //adjust delay until 135 degrees
    stop(mL, mR);                       //stop motors
    toggle_left_indicators();           //indicate left for road safety
}

/*******************************************************************************
 * Function to make the robot turn right for 135 degrees
*******************************************************************************/
void turnRight135(DC_motor *mL, DC_motor *mR)
{
    toggle_right_indicators();          //indicate right for road safety
    turnRight(mL, mR);                  //set motors to continuously turn right
    __delay_ms(turnRight135Delay);      //adjust delay until 135 degrees
    stop(mL, mR);                       //stop motors
    toggle_right_indicators();          //indicate right for road safety
}

/*******************************************************************************
 * Function to make the robot turn 180 degrees
*******************************************************************************/
void UTurn(DC_motor *mL, DC_motor *mR)
{
    toggle_left_indicators();           //hazard warning lights for road safety
    toggle_right_indicators();          //hazard warning lights for road safety
    turnLeft(mL, mR);                   //set motors to continuously turn left
    __delay_ms(turn180Delay);           //adjust delay until 180 degrees
    stop(mL, mR);                       //stop motors
    toggle_left_indicators();           //hazard warning lights for road safety
    toggle_right_indicators();          //hazard warning lights for road safety
}

/*******************************************************************************
 * Function to make the robot reverse from the wall of a square to the centre
*******************************************************************************/
void headbuttReverse(DC_motor *mL, DC_motor *mR)
{
    toggle_brake_lights();              //reversing lights for road safety
    fullSpeedReverse(mL, mR);           //set motors to continuously go reverse
    __delay_ms(headbuttDelay);          //adjust delay until centre of square
    stop(mL, mR);                       //stop motors
    toggle_brake_lights();              //reversing lights for road safety
}

/*******************************************************************************
 * Function to make the robot reverse 1 square length
*******************************************************************************/
void squareReverse(DC_motor *mL, DC_motor *mR)
{
    toggle_brake_lights();              //reversing lights for road safety
    fullSpeedReverse(mL, mR);           //set motors to continuously go reverse
    __delay_ms(squareDelay);            //adjust delay until 1 square length
    stop(mL, mR);                       //stop motors
    toggle_brake_lights();              //reversing lights for road safety
}