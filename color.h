#ifndef _color_H
#define _color_H

#include <xc.h>
#include "i2c.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

unsigned int wall_threshold = 300;

//definition of RGB structure
typedef struct RGBC_val { 
	unsigned int R;
	unsigned int G;
	unsigned int B;
    unsigned int C;
} RGBC_val;

//definition of HSV structure
typedef struct HSV_val {
    unsigned int H;
    unsigned int S;
    unsigned int V;
} HSV_val;

/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

/********************************************//**
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 ***********************************************/
void color_writetoaddr(char address, char value);

/********************************************//**
 *  Function to read the red channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Red(void);

/********************************************//**
 *  Function to read the green channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Green(void);

/********************************************//**
 *  Function to read the blue channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Blue(void);

/********************************************//**
 *  Function to read the clear channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Clear(void);

/********************************************//**
 *  Function to update structure variables (RGBC) in RGBCval
 *	Updates variables R, G, B, C with the values read 
 ***********************************************/
void getRGBCval(struct RGBC_val *p);

void wait_for_wall(struct RGBC_val *p);
unsigned int max(unsigned int a, unsigned int b);
unsigned int min(unsigned int a, unsigned int b);
unsigned int maxRGB(struct RGBC_val *p);
unsigned int minRGB(struct RGBC_val *p);
void scaleRGB(struct RGBC_val *p);
void getHSVval(struct HSV_val *p1, struct RGBC_val *p2);
#endif
