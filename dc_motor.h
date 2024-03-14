#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "buggy_lights.h"

#define _XTAL_FREQ 64000000

//define DC_motor structure
typedef struct DC_motor {
    char power;                         //motor power: out of 100
    char direction;                     //motor direction: forward(1), reverse(0)
    char brakemode;                     //short or fast decay (brake or coast)
    unsigned int PWMperiod;             //base period of PWM cycle
    unsigned char *posDutyHighByte;     //PWM duty address for motor +ve side
    unsigned char *negDutyHighByte;     //PWM duty address for motor -ve side
} DC_motor;

unsigned char topGearLeft = 20;             //power from 0 to 100
unsigned char topGearRight = 20;            //power from 0 to 100

unsigned char bottomGearLeft = 20;          //power from 0 to 100
unsigned char bottomGearRight = 21;         //power from 0 to 100

unsigned char turningLeftGear = 40;         //power from 0 to 100
unsigned char turningRightGear = 40;        //power from 0 to 100

//maximum value for __delay_ms(); is 3153
unsigned char rampDelay = 8;                //delay in ms

unsigned int turnLeft90Delay = 84;         //delay in ms
unsigned int turnRight90Delay = 92;        //delay in ms
unsigned int turnLeft135Delay = 250;        //delay in ms
unsigned int turnRight135Delay = 262;       //delay in ms
unsigned int turn180Delay = 380;            //delay in ms

unsigned int headbuttDelay = 820;            //delay in ms
unsigned int squareDelay = 2440;             //delay in ms

void initDCmotorsPWM(unsigned int PWMperiod);
void setMotorPWM(DC_motor *m);

void stop(DC_motor *mL, DC_motor *mR);
void turnLeft(DC_motor *mL, DC_motor *mR);
void turnRight(DC_motor *mL, DC_motor *mR);
void fullSpeedAhead(DC_motor *mL, DC_motor *mR);
void fullSpeedReverse(DC_motor *mL, DC_motor *mR);

void turnLeft90(DC_motor *mL, DC_motor *mR);
void turnRight90(DC_motor *mL, DC_motor *mR);
void turnLeft135(DC_motor *mL, DC_motor *mR);
void turnRight135(DC_motor *mL, DC_motor *mR);
void UTurn(DC_motor *mL, DC_motor *mR);
void headbuttReverse(DC_motor *mL, DC_motor *mR);
void squareReverse(DC_motor *mL, DC_motor *mR);

#endif