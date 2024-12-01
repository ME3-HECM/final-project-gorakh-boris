#include <xc.h>
#include "color.h"

/*******************************************************************************
 * Function to initialise the colour click module using I2C
*******************************************************************************/
void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();            //Initialise i2c Master

    //set device PON
	color_writetoaddr(0x00, 0x01);
    __delay_ms(3);                  //need to wait 3ms for everthing to start up
    
    //turn on device ADC
	color_writetoaddr(0x00, 0x03);

    //set integration time
	color_writetoaddr(0x01, 0xD5);
}

/*******************************************************************************
 * Function to write to the colour click module
 * Address is the register within the colour click to write to
 * Value is the value that will be written to that address
*******************************************************************************/
void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();                   //Start condition
    I2C_2_Master_Write(0x52 | 0x00);        //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);     //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();                    //Stop condition
}

/*******************************************************************************
 * Function to read the red channel
 * Returns a 16 bit ADC value representing colour intensity
*******************************************************************************/
unsigned int color_read_Red(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();                   //Start condition
	I2C_2_Master_Write(0x52 | 0x00);        //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x16);        //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();                // start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);        //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);               //read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8);    //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();                    //Stop condition
	return tmp;
}

/*******************************************************************************
 * Function to read the green channel
 * Returns a 16 bit ADC value representing colour intensity
*******************************************************************************/
unsigned int color_read_Green(void)
{
    unsigned int tmp;
	I2C_2_Master_Start();                   //Start condition
	I2C_2_Master_Write(0x52 | 0x00);        //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x18);        //command (auto-increment protocol transaction) + start at GREEM low register
	I2C_2_Master_RepStart();                // start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);        //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);               //read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8);    //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();                    //Stop condition
	return tmp;
}

/*******************************************************************************
 * Function to read the blue channel
 * Returns a 16 bit ADC value representing colour intensity
*******************************************************************************/
unsigned int color_read_Blue(void)
{
    unsigned int tmp;
	I2C_2_Master_Start();                   //Start condition
	I2C_2_Master_Write(0x52 | 0x00);        //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x1A);        //command (auto-increment protocol transaction) + start at BLUE low register
	I2C_2_Master_RepStart();                // start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);        //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);               //read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8);    //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();                    //Stop condition
	return tmp;
}

/*******************************************************************************
 * Function to read the clear channel
 * Returns a 16 bit ADC value representing colour intensity
*******************************************************************************/
unsigned int color_read_Clear(void)
{
    unsigned int tmp;
	I2C_2_Master_Start();                   //Start condition
	I2C_2_Master_Write(0x52 | 0x00);        //7 bit address + Write mode
	I2C_2_Master_Write(0xA0 | 0x14);        //command (auto-increment protocol transaction) + start at CLEAR low register
	I2C_2_Master_RepStart();                // start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);        //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);               //read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8);    //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();                    //Stop condition
	return tmp;
}

/*******************************************************************************
 * Function to update structure variables (RGBC) in RGBCval
 * Updates variables R, G, B, C with the values read
*******************************************************************************/
void read_RGBC(struct RGBC_val *p)
{
    p->R = color_read_Red();
    p->G = color_read_Green();
    p->B = color_read_Blue();
    p->C = color_read_Clear();
    
}

/*******************************************************************************
 * Function to sample RGBC values multiple times and take an average
*******************************************************************************/
void average_RGBC(struct RGBC_val *p)
{
    //temporary RGBC variables
    unsigned long temp_R = 0;
    unsigned long temp_G = 0;
    unsigned long temp_B = 0;
    unsigned long temp_C = 0;
    
    //loop sample_count times
    for (unsigned char i = 0; i < sample_count; i++) {
        
        //take colour measurement
        read_RGBC(p);
        
        //add colour measurement to temporary variables
        temp_R += (unsigned long)p->R;
        temp_G += (unsigned long)p->G;
        temp_B += (unsigned long)p->B;
        temp_C += (unsigned long)p->C;
    }
    
    //divide temporary variables by sample_count
    temp_R /= (unsigned long)sample_count;
    temp_G /= (unsigned long)sample_count;
    temp_B /= (unsigned long)sample_count;
    temp_C /= (unsigned long)sample_count;
    
    //assign temporary variables to input RGBC_val structure
    p->R = (unsigned int)temp_R;
    p->G = (unsigned int)temp_G;
    p->B = (unsigned int)temp_B;
    p->C = (unsigned int)temp_C;
}

/*******************************************************************************
 * Function to wait until clear channel reaches threshold or lost flag appears
*******************************************************************************/
void wait_for_wall(struct RGBC_val *p, unsigned char loss)
{
    //loop until lost_flag raised
    while (!loss) {
        
        //read RGBC values from colour click
        read_RGBC(p);
        
        //when clear channel drops below threshold, break loop and proceed
        if (p->C < wall_threshold_clear) {break;}
    }
}

/*******************************************************************************
 * Function to return maximum value within RGB
*******************************************************************************/
unsigned int max_RGB(struct RGBC_val *p)
{
    unsigned int max_val = p->R;            //set red as initial max value
    if (p->G > max_val) {max_val = p->G;}   //if green is larger, set new max
    if (p->B > max_val) {max_val = p->B;}   //if blue is larger, set new max
    return max_val;
}

/*******************************************************************************
 * Function to return minimum value within RGB
*******************************************************************************/
unsigned int min_RGB(struct RGBC_val *p)
{
    unsigned int min_val = p->R;            //set red as initial min value
    if (p->G < min_val) {min_val = p->G;}   //if green is smaller, set new min
    if (p->B < min_val) {min_val = p->B;}   //is blue is smaller, set new min
    return min_val;
}

/*******************************************************************************
 * Function to scale and adjust values within RGB
*******************************************************************************/
void scale_RGB(struct RGBC_val *p)
{
    p->R *= 0.5;                            //scale red value by chosen factor
    p->G *= 0.7;                            //scale green value by chosen factor
    p->B *= 1;                              //scale blue value by chosen factor
}

/*******************************************************************************
 * Function to calculate HSV values based on input RGB values
 * https://en.wikipedia.org/wiki/HSL_and_HSV
 * https://www.rapidtables.com/convert/color/rgb-to-hsv.html
*******************************************************************************/
void convert_HSV(struct HSV_val *p1, struct RGBC_val *p2)
{
    unsigned int hue = 0;                   //hue value
    unsigned int sat = 0;                   //saturation value
    unsigned int hue_scale = 60;            //scaling factor for hue
    unsigned int sat_scale = 100;           //scaling factor for saturation
    
    unsigned int M = max_RGB(p2);            //maximum value within RGB
    unsigned int m = min_RGB(p2);            //minimum value within RGB
    unsigned int C = M - m;                 //range of RGB (maximum - minimum)
    
    if (C == 0) {
        hue = 0;
    } else {
        /***************************************************************
         * For each of the six cases below, the commented line shows the
         * actual equation to calculate the hue value
         * 
         * The lines of code underneath is written to calculate the hue
         * value using a temporary long integer to prevent overflow
         * 
         * Refer to README for how the equations were determined
        ***************************************************************/
        if (M == p2->R) {
            if (p2->G >= p2->B) {
                //hue = (hue_scale * (0 * C + (p2->G - p2->B))) / C;
                unsigned long temp;
                temp = (unsigned long)C;
                temp *= 0;
                temp += (unsigned long)(p2->G - p2->B);
                temp *= (unsigned long)hue_scale;
                temp /= (unsigned long)C;
                hue = (unsigned int)temp;
            } else {
                //hue = (hue_scale * (6 * C - (p2->B - p2->G))) / C;
                unsigned long temp;
                temp = (unsigned long)C;
                temp *= 6;
                temp -= (unsigned long)(p2->B - p2->G);
                temp *= (unsigned long)hue_scale;
                temp /= (unsigned long)C;
                hue = (unsigned int)temp;
            }
        }
        if (M == p2->G) {
            if (p2->B >= p2->R) {
                //hue = (hue_scale * (2 * C + (p2->B - p2->R))) / C;
                unsigned long temp;
                temp = (unsigned long)C;
                temp *= 2;
                temp += (unsigned long)(p2->B - p2->R);
                temp *= (unsigned long)hue_scale;
                temp /= (unsigned long)C;
                hue = (unsigned int)temp;
            } else {
                //hue = (hue_scale * (2 * C - (p2->R - p2->B))) / C;
                unsigned long temp;
                temp = (unsigned long)C;
                temp *= 2;
                temp -= (unsigned long)(p2->R - p2->B);
                temp *= (unsigned long)hue_scale;
                temp /= (unsigned long)C;
                hue = (unsigned int)temp;
            }
        }
        if (M == p2->B) {
            if (p2->R >= p2->G) {
                //hue = (hue_scale * (4 * C + (p2->R - p2->G))) / C;
                unsigned long temp;
                temp = (unsigned long)C;
                temp *= 4;
                temp += (unsigned long)(p2->R - p2->G);
                temp *= (unsigned long)hue_scale;
                temp /= (unsigned long)C;
                hue = (unsigned int)temp;
            } else {
                //hue = (hue_scale * (4 * C - (p2->G - p2->R))) / C;
                unsigned long temp;
                temp = (unsigned long)C;
                temp *= 4;
                temp -= (unsigned long)(p2->G - p2->R);
                temp *= (unsigned long)hue_scale;
                temp /= (unsigned long)C;
                hue = (unsigned int)temp;
            }
        }
    }
    
    if (M == 0) {
        sat = 0;
    } else {
        //sat = sat_scale * C / M;
        unsigned long temp;
        temp = (unsigned long)C;
        temp *= (unsigned long)sat_scale;
        temp /= (unsigned long)M;
        sat = (unsigned int)temp;
    }
    
    p1->H = hue;                            //assign hue
    p1->S = sat;                            //assign saturation
    p1->V = M;                              //assign value = maximum RGB
}

/*******************************************************************************
 * Function that matches RGBC and HSV values to a colour
 * The function compares the readings to a set of upper and lower bound readings
 * Colour is outputted as a number from 1 - 9
*******************************************************************************/
unsigned char colour_to_key(struct HSV_val *p1, struct RGBC_val *p2)
{
    //initial zero value for output
    unsigned char key = 0;
    
    //red (1)
    if ( ((293 <= p1->H) && (p1->H <= 360)) && 
         ((46 <= p1->S) && (p1->S <= 90))) { 
        key = 1;
    }
    
    //green (2)
    if ( ((113 <= p1->H) && (p1->H <= 153)) && 
         ((13 <= p1->S) && (p1->S <= 43))) {
        key = 2;
    }

    //blue (3)
    if ( ((180 <= p1->H) && (p1->H <= 244)) && 
         ((13 <= p1->S) && (p1->S <= 56))) {
        key = 3;
    }

    //yellow (4)
    if ( ((10 <= p1->H) && (p1->H <= 31)) && 
         ((17 <= p1->S) && (p1->S <= 32))) {
        key = 4;
    }

    //pink (5)
    if ( ((279 <= p1->H) && (p1->H <= 360)) && 
         ((11 <= p1->S) && (p1->S <= 15))) {
        key = 5;
    }

    //orange (6)
    if ( ((298 <= p1->H) && (p1->H <= 360)) && 
         ((20 <= p1->S) && (p1->S <= 45))) {
        key = 6;
    }

    //cyan (7)
    if ( ((154 <= p1->H) && (p1->H <= 207)) && 
         ((13 <= p1->S) && (p1->S <= 45))) {
        key = 7;
    }

    //white (8)
    if ( ((3000 <= p2->C) && (p2->C <= 40000)) && 
         ((p1->S <= 10))) {
        key = 8;
    }

    //black (9)
    if ( ((1000 <= p2->C) && (p2->C <= 2600)) && 
         ((p1->S <= 12))) {
        key = 9;
    }
    
    return key;
}