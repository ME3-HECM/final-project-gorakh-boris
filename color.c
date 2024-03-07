#include <xc.h>
#include "color.h"
#include "i2c.h"

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

void getRGBCval(struct RGBC_val *p)// function getRGB_val updates variables in the structure RGBC 
{
    p->R = color_read_Red();        //value of p = color_read_Red(), stored in RBGC_val variable R
    p->B = color_read_Blue();       //value of p = color_read_Blue(), stored in RBGC_val variable B
    p->G = color_read_Green();      //value of p = color_read_Green(), stored in RBGC_val variable G
    p->C = color_read_Clear();      //value of p = color_read_Clear(), stored in RBGC_val variable C
}

/*!
 * This is a function returns the maximum value from two inputs
 * The parameter a is a  signed integer number to be compared
 * The parameter b is a signed integer number to be compared
 * The function returns the integer that is bigger 
 */
int  max(int a, int b) 
{
    int max_val;                //max_val is created to hold the value of the larger integer
                                //this is to ensure the function has only one return
    if (a > b) {max_val = a;}
    
    else {max_val = b;}
    
    return max_val;
}

/*!
 * Function returns the minimum value from two inputs
 * The parameter a is a  signed integer number to be compared
 * The parameter b is a signed integer number to be compared
 * The function returns the integer that is smaller 
 */
int min(int a, int b)
{
    int min_val;                //min_val is created to hold the value of the smaller integer
                                //this is to ensure the function has only one return
    if (a < b) {min_val = a;}
    
    else {min_val = b;}
    
    return min_val;
}

/*!
 * Function returns the Hue value from the RGB value
 * The function returns an integer that corresponds to the hue
 * 

 */


/*!
 * The function updates the HSV_val structure updating the numbers for Hue, Saturation, Value 
 * The parameter HSV_val *p is a pointer that points to the Hue, Saturation, Value variables in the structure HSV_val
 * Notes: the equation was obtained from https://en.wikipedia.org/wiki/HSL_and_HSV
 * However, the equation for Hue has been modified so that we do not use floats
 * The numerator has been multiplied by 100 
 * so the hue values will range from 0-36000 instead of 0-360
 */
void getHSVval(struct HSV_val *p1,struct RGBC_val *p2)
{
    int maxRGB;
    int minRGB;
    int range;
    int hue;
    int saturation;
    
 
    
    maxRGB = max(max(p2->R,p2->G),p2->B);
    minRGB = min(min(p2->R,p2->G),p2->B);
    range = maxRGB - minRGB;
    
    if (range == 0) {
        hue = 0;
    }
    
    if (maxRGB == (p2->R)) {
        hue = 60*(((((p2->G)-(p2->B))*10)/range)%600);
    }
    
    if (maxRGB == (p2->G)){
        hue = 60*(((((p2->B)-(p2->R))*10)/range)+200);
    }
    
    if (maxRGB == (p2->G)){
        hue = 60*(((((p2->R)-(p2->G))*10)/range)+400);
    }
    
    if (maxRGB == 0) {saturation = 0;}
    
    if (maxRGB != 0) {saturation = (range*10)/maxRGB;}
    
    p1->H = hue;
    p1->S = saturation;
    p1->V = maxRGB;
    
}