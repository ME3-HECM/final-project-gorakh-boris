#include <xc.h>
#include "color.h"

void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master

     //set device PON
	 color_writetoaddr(0x00, 0x01);
    __delay_ms(3); //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	color_writetoaddr(0x00, 0x03);

    //set integration time
	color_writetoaddr(0x01, 0xD5);
}

void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

unsigned int color_read_Red(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Blue(void)
{
    unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x1A);    //command (auto-increment protocol transaction) + start at BLUE low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Green(void)
{
    unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x18);    //command (auto-increment protocol transaction) + start at GREEM low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

unsigned int color_read_Clear(void)
{
    unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x14);    //command (auto-increment protocol transaction) + start at CLEAR low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

void getRGBCval(struct RGBC_val *p) //function getRGB_val updates variables in the structure RGBC 
{
    p->R = color_read_Red();        //value of p = color_read_Red(), stored in RBGC_val variable R
    p->B = color_read_Blue();       //value of p = color_read_Blue(), stored in RBGC_val variable B
    p->G = color_read_Green();      //value of p = color_read_Green(), stored in RBGC_val variable G
    p->C = color_read_Clear();      //value of p = color_read_Clear(), stored in RBGC_val variable C
    
}

void wait_for_wall(struct RGBC_val *p)
{
    while (1) {
        getRGBCval(p);
        if (p->C < wall_threshold) {
            break;
        }
    }
    LATDbits.LATD7 = !LATDbits.LATD7;       //toggle LED for debugging
}

unsigned int max(unsigned int a, unsigned int b) 
{
    unsigned int max_val = b;
    if (a > b) {max_val = a;}
    return max_val;
}

unsigned int min(unsigned int a, unsigned int b)
{
    unsigned int min_val = b;
    if (a < b) {min_val = a;}
    return min_val;
}

unsigned int maxRGB(struct RGBC_val *p)
{
    return(max(max(p->R,p->G),p->B));
}

unsigned int minRGB(struct RGBC_val *p)
{
    return(min(min(p->R,p->G),p->B));
}

//https://en.wikipedia.org/wiki/HSL_and_HSV
void getHSVval(struct HSV_val *p1,struct RGBC_val *p2)
{
    unsigned int z = 60;        //scaling factor for hue and saturation
    unsigned int H = 0;
    unsigned int S = 0;
    
    unsigned int RR = (p2->R);
    unsigned int GG = (p2->G);
    unsigned int BB = (p2->B);
    
    unsigned int M = maxRGB(p2);
    unsigned int m = minRGB(p2);
    unsigned int C = M - m;
    
    if (C == 0) {
        H = 0;
    } else {
        if (M == RR) {
            if (GG >= BB) {
                //H = (z * (0 * C + (p2->G - p2->B))) / C;
                unsigned long temp;
                temp = (unsigned long)C;
                temp *= 0;
                temp = (unsigned long)(GG - BB);
                temp *= (unsigned long)z;
                temp /= (unsigned long)C;
                H = (unsigned int)temp;
            } else {
                //H = (z * (6 * C - (BB - GG))) / C;
                unsigned long temp;
                temp = (unsigned long)C;
                temp *= 6;
                temp -= (unsigned long)(BB - GG);
                temp *= (unsigned long)z;
                temp /= (unsigned long)C;
                H = (unsigned int)temp;
            }
        }
        if (M == p2->G) {
            if (p2->B >= p2->R) {
                H = (z * (2 * C + (p2->B - p2->R))) / C;
            } else {
                H = (z * (2 * C - (p2->R - p2->B))) / C;
            }
        }
        if (M == p2->B) {
            if (p2->R >= p2->G) {
                H = (z * (4 * C + (p2->R - p2->G))) / C;
            } else {
                H = (z * (4 * C - (p2->G - p2->R))) / C;
            }
        }
    }
    
    if (M == 0) {
        S = 0;
    } else {
        //S = z * C / M;
        unsigned long temp;
        temp = (unsigned long)C;
        temp *= (unsigned long)z;
        temp /= (unsigned long)M;
        S = (unsigned int)temp;
    }
    
    p1->H = H;
    p1->S = S;
    p1->V = M;
    
}