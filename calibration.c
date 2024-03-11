#include <xc.h>
#include "calibration.h"

/*******************************************************************************
 * Function to call manoeuvres.c functions for testing and debugging
*******************************************************************************/
void test_manoeuvres(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
{
    //card_red(mL, mR, backtrack);
    //card_green(mL, mR, backtrack);
    //card_blue(mL, mR, backtrack);
    //card_yellow(mL, mR, backtrack);
    //card_pink(mL, mR, backtrack);
    //card_orange(mL, mR, backtrack);
    //card_cyan(mL, mR, backtrack);
    //card_white(mL, mR);
    
    pick_card(mL, mR, backtrack, 1);        //red
    pick_card(mL, mR, backtrack, 2);        //green
    pick_card(mL, mR, backtrack, 3);        //blue
    pick_card(mL, mR, backtrack, 4);        //yellow
    pick_card(mL, mR, backtrack, 5);        //pink
    pick_card(mL, mR, backtrack, 6);        //orange
    pick_card(mL, mR, backtrack, 7);        //cyan
    pick_card(mL, mR, backtrack, 8);        //white
}