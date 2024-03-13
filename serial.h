#ifndef _SERIAL_H
#define _SERIAL_H

#include <xc.h>
#include "color.h"

#define _XTAL_FREQ 64000000

void initUSART4(void);
void sendCharSerial4(char charToSend);
void sendStringSerial4(char *string);
void sendIntSerial4(int integer);
void sendArrayCharSerial4(unsigned char *arr);
void sendRGBCvalSerial4(RGBC_val *col_val);
void sendHSVvalSerial4(HSV_val *col_val);

#endif