#ifndef _color_H
#define _color_H

#include <xc.h>
#include "i2c.h"

#define _XTAL_FREQ 64000000

unsigned char sample_count = 20;
unsigned int wall_threshold_blue = 30;

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

void average_RGBC(struct RGBC_val *p);
void wait_for_wall(struct RGBC_val *p, unsigned char loss);
unsigned int max_RGB(struct RGBC_val *p);
unsigned int min_RGB(struct RGBC_val *p);
void scale_RGB(struct RGBC_val *p);
void convert_HSV(struct HSV_val *p1, struct RGBC_val *p2);
unsigned char colour_to_key(struct HSV_val *p1, struct RGBC_val *p2);
#endif