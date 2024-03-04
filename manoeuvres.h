/* 
 * File:   manoeuvres.h
 * Author: boris
 *
 * Created on 26 February 2024, 13:38
 */

#ifndef MANOEUVRES_H
#define	MANOEUVRES_H

#include "dc_motor.h"

void cardRed(DC_motor *mL, DC_motor *mR, unsigned char backtrack);          //turn right 90
void cardGreen(DC_motor *mL, DC_motor *mR, unsigned char backtrack);        //turn left 90
void cardBlue(DC_motor *mL, DC_motor *mR, unsigned char backtrack);         //turn 180
void cardYellow(DC_motor *mL, DC_motor *mR, unsigned char backtrack);       //reverse 1 square and turn right 90
void cardPink(DC_motor *mL, DC_motor *mR, unsigned char backtrack);         //reverse 1 square and turn left 90
void cardOrange(DC_motor *mL, DC_motor *mR, unsigned char backtrack);       //turn right 135
void cardCyan(DC_motor *mL, DC_motor *mR, unsigned char backtrack);         //turn left 135
void cardWhite(DC_motor *mL, DC_motor *mR);         //finish and return home
                                                    //buggy does U turn
                                                    //travels in forward direction when backtracking
void pickCard(DC_motor *mL, DC_motor *mR, unsigned char backtrack, unsigned char key);

#endif