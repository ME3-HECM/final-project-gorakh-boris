/* 
 * File:   buggy_lights.h
 * Author: boris
 *
 * Created on 26 February 2024, 12:20
 */

#ifndef BUGGY_LIGHTS_H
#define	BUGGY_LIGHTS_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void buggy_lights_init(void);

void toggle_brake_lights(void);
void toggle_main_beam(void);
void toggle_left_indicators(void);
void toggle_right_indicators(void);

void toggle_tricolour_LED(void);

#endif