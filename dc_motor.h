#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>

#define _XTAL_FREQ 64000000

typedef struct DC_motor { //definition of DC_motor structure
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    char brakemode;		// short or fast decay (brake or coast)
    unsigned int PWMperiod; //base period of PWM cycle
    unsigned char *posDutyHighByte; //PWM duty address for motor +ve side
    unsigned char *negDutyHighByte; //PWM duty address for motor -ve side
} DC_motor;

unsigned char rampDelay = 8;       //delay in ms

unsigned char topGear = 30;         //speed from 0 to 100
unsigned char topAdjustPower = 2;       //speed from 0 to 100
unsigned char topAdjustSide = 1;    //0 for adding power to left, 1 for adding power to right

unsigned char turningGear = 42;     //speed from 0 to 100

unsigned int turnLeft90Delay = 175;         //delay in ms
unsigned int turnRight90Delay = 175;        //delay in ms
unsigned int turnLeft135Delay = 300;        //delay in ms
unsigned int turnRight135Delay = 300;       //delay in ms
unsigned int turn180Delay = 510;            //delay in ms

//function prototypes
void initDCmotorsPWM(unsigned int PWMperiod); // function to setup PWM
void setMotorPWM(DC_motor *m);

void stop(DC_motor *mL, DC_motor *mR);
void turnLeft(DC_motor *mL, DC_motor *mR);
void turnRight(DC_motor *mL, DC_motor *mR);
void fullSpeedAhead(DC_motor *mL, DC_motor *mR);

void turnLeft90(DC_motor *mL, DC_motor *mR);
void turnRight90(DC_motor *mL, DC_motor *mR);
void turnLeft135(DC_motor *mL, DC_motor *mR);
void turnRight135(DC_motor *mL, DC_motor *mR);
void UTurn(DC_motor *mL, DC_motor *mR);

#endif