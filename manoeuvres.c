#include <xc.h>
#include "manoeuvres.h"

void cardRed(DC_motor *mL, DC_motor *mR, unsigned char backtrack)          //turn right 90
{
    switch (backtrack) {
        case 0:             //forward operation
            headbuttReverse(mL, mR);
            turnRight90(mL, mR);
            break;
        case 1:             //backtracking operation
            turnLeft90(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

void cardGreen(DC_motor *mL, DC_motor *mR, unsigned char backtrack)        //turn left 90
{
    switch (backtrack) {
        case 0:             //forward operation
            headbuttReverse(mL, mR);
            turnLeft90(mL, mR);
            break;
        case 1:             //backtracking operation
            turnRight90(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

void cardBlue(DC_motor *mL, DC_motor *mR, unsigned char backtrack)         //turn 180
{
    switch (backtrack) {
        case 0:             //forward operation
            headbuttReverse(mL, mR);
            UTurn(mL, mR);
            break;
        case 1:             //backtracking operation
            UTurn(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

void cardYellow(DC_motor *mL, DC_motor *mR, unsigned char backtrack)       //reverse 1 square and turn right 90
{
    switch (backtrack) {
        case 0:             //forward operation
            headbuttReverse(mL, mR);
            squareReverse(mL, mR);
            turnRight90(mL, mR);
            break;
        case 1:             //backtracking operation
            turnLeft90(mL, mR);
            squareReverse(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

void cardPink(DC_motor *mL, DC_motor *mR, unsigned char backtrack)         //reverse 1 square and turn left 90
{
    switch (backtrack) {
        case 0:             //forward operation
            headbuttReverse(mL, mR);
            squareReverse(mL, mR);
            turnLeft90(mL, mR);
            break;
        case 1:             //backtracking operation
            turnRight90(mL, mR);
            squareReverse(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

void cardOrange(DC_motor *mL, DC_motor *mR, unsigned char backtrack)       //turn right 135
{
    switch (backtrack) {
        case 0:             //forward operation
            headbuttReverse(mL, mR);
            turnRight135(mL, mR);
            break;
        case 1:             //backtracking operation
            turnLeft135(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

void cardCyan(DC_motor *mL, DC_motor *mR, unsigned char backtrack)         //turn left 135
{
    switch (backtrack) {
        case 0:             //forward operation
            headbuttReverse(mL, mR);
            turnLeft135(mL, mR);
            break;
        case 1:             //backtracking operation
            turnRight135(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

void cardWhite(DC_motor *mL, DC_motor *mR)        //finish and return home
{
    headbuttReverse(mL, mR);
    UTurn(mL, mR);
    headbuttReverse(mL, mR);
}