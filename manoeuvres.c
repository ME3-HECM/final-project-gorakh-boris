#include <xc.h>
#include "manoeuvres.h"

/*******************************************************************************
 * Red card signals a 90 degrees right turn
*******************************************************************************/
void card_red(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
{
    switch (backtrack) {
        case 0:         //forward operation
            headbuttReverse(mL, mR);
            turnRight90(mL, mR);
            break;
        case 1:         //backtracking operation
            turnLeft90(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

/*******************************************************************************
 * Green card signals a 90 degrees left turn
*******************************************************************************/
void card_green(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
{
    switch (backtrack) {
        case 0:         //forward operation
            headbuttReverse(mL, mR);
            turnLeft90(mL, mR);
            break;
        case 1:         //backtracking operation
            turnRight90(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

/*******************************************************************************
 * Blue card signals a 180 degrees U turn
*******************************************************************************/
void card_blue(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
{
    switch (backtrack) {
        case 0:         //forward operation
            headbuttReverse(mL, mR);
            UTurn(mL, mR);
            break;
        case 1:         //backtracking operation
            UTurn(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

/*******************************************************************************
 * Yellow card signals a 1 square-length reverse and a 90 degrees right turn
*******************************************************************************/
void card_yellow(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
{
    switch (backtrack) {
        case 0:         //forward operation
            headbuttReverse(mL, mR);
            squareReverse(mL, mR);
            turnRight90(mL, mR);
            break;
        case 1:         //backtracking operation
            turnLeft90(mL, mR);
            squareReverse(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

/*******************************************************************************
 * Pink card signals a 1 square-length reverse and a 90 degrees left turn
*******************************************************************************/
void card_pink(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
{
    switch (backtrack) {
        case 0:         //forward operation
            headbuttReverse(mL, mR);
            squareReverse(mL, mR);
            turnLeft90(mL, mR);
            break;
        case 1:         //backtracking operation
            turnRight90(mL, mR);
            squareReverse(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

/*******************************************************************************
 * Orange card signals a 135 degrees right turn
*******************************************************************************/
void card_orange(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
{
    switch (backtrack) {
        case 0:         //forward operation
            headbuttReverse(mL, mR);
            turnRight135(mL, mR);
            break;
        case 1:         //backtracking operation
            turnLeft135(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

/*******************************************************************************
 * Cyan card signals a 135 degrees left turn
*******************************************************************************/
void card_cyan(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
{
    switch (backtrack) {
        case 0:         //forward operation
            headbuttReverse(mL, mR);
            turnLeft135(mL, mR);
            break;
        case 1:         //backtracking operation
            turnRight135(mL, mR);
            headbuttReverse(mL, mR);
            break;
    }
}

/*******************************************************************************
 * White card signals finishing the maze and return home
 * Buggy performs a U turn and travels in forward direction when returning home
*******************************************************************************/
void card_white(DC_motor *mL, DC_motor *mR)
{
    headbuttReverse(mL, mR);
    UTurn(mL, mR);
    headbuttReverse(mL, mR);
}

/*******************************************************************************
 * Function to pick the correct card manoeuvre based on a key (1-8)
*******************************************************************************/
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