#include <xc.h>
#include "manoeuvres.h"

void card_red(DC_motor *mL, DC_motor *mR, unsigned char backtrack)              //turn right 90
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

void card_green(DC_motor *mL, DC_motor *mR, unsigned char backtrack)         //turn left 90
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

void card_blue(DC_motor *mL, DC_motor *mR, unsigned char backtrack)          //turn 180
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

void card_yellow(DC_motor *mL, DC_motor *mR, unsigned char backtrack)        //reverse 1 square and turn right 90
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

void card_pink(DC_motor *mL, DC_motor *mR, unsigned char backtrack)          //reverse 1 square and turn left 90
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

void card_orange(DC_motor *mL, DC_motor *mR, unsigned char backtrack)        //turn right 135
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

void card_cyan(DC_motor *mL, DC_motor *mR, unsigned char backtrack)          //turn left 135
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

void card_white(DC_motor *mL, DC_motor *mR)                                  //finish and return home
{
    headbuttReverse(mL, mR);
    UTurn(mL, mR);
    headbuttReverse(mL, mR);
}

void pick_card(DC_motor *mL, DC_motor *mR, unsigned char backtrack, unsigned char key)
{
    switch (key) {
        case 1:         //red
            card_red(mL, mR, backtrack);
            break;
        case 2:         //green
            card_green(mL, mR, backtrack);
            break;
        case 3:         //blue
            card_blue(mL, mR, backtrack);
            break;
        case 4:         //yellow
            card_yellow(mL, mR, backtrack);
            break;
        case 5:         //pink
            card_pink(mL, mR, backtrack);
            break;
        case 6:         //orange
            card_orange(mL, mR, backtrack);
            break;
        case 7:         //cyan
            card_cyan(mL, mR, backtrack);
            break;
        case 8:         //white
            card_white(mL, mR);
            break;
    }
}

