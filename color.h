#ifndef _color_H
#define _color_H

#include <xc.h>
#include "i2c.h"

#define _XTAL_FREQ 64000000

unsigned int wall_threshold = 300;

//define RGBC structure
typedef struct RGBC_val { 
	unsigned int R;
	unsigned int G;
	unsigned int B;
    unsigned int C;
} RGBC_val;

//define HSV structure
typedef struct HSV_val {
    unsigned int H;
    unsigned int S;
    unsigned int V;
} HSV_val;

void color_click_init(void);
void color_writetoaddr(char address, char value);
unsigned int color_read_Red(void);
unsigned int color_read_Green(void);
unsigned int color_read_Blue(void);
unsigned int color_read_Clear(void);
void getRGBCval(struct RGBC_val *p);

void wait_for_wall(struct RGBC_val *p);
unsigned int maxRGB(struct RGBC_val *p);
unsigned int minRGB(struct RGBC_val *p);
void scaleRGB(struct RGBC_val *p);
void getHSVval(struct HSV_val *p1, struct RGBC_val *p2);
#endif