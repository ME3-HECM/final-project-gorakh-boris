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
 * address is the register within the colour click to write to
 * value is the value that will be written to that address
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
void getRGBCval(struct RGBC_val *p)
{
    p->R = color_read_Red();                //value of p = color_read_Red(), stored in RBGC_val variable R
    p->G = color_read_Green();              //value of p = color_read_Green(), stored in RBGC_val variable G
    p->B = color_read_Blue();               //value of p = color_read_Blue(), stored in RBGC_val variable B
    p->C = color_read_Clear();              //value of p = color_read_Clear(), stored in RBGC_val variable C
    
}

/*******************************************************************************
 * Function to wait until clear channel reaches threshold or lost flag appears
*******************************************************************************/
void wait_for_wall(struct RGBC_val *p, unsigned char loss)
{
    while (!loss) {                     //loop while lost flag isn't raised
        getRGBCval(p);                  //read RGBC values from colour click
        if (p->C < wall_threshold) {    //if clear channel drops below threshold
            break;                      //break loop and proceed
        }
    }
    LATDbits.LATD7 = !LATDbits.LATD7;   //toggle LED for debugging
}

/*******************************************************************************
 * Function to return maximum value within RGB
*******************************************************************************/
unsigned int maxRGB(struct RGBC_val *p)
{
    unsigned int max_val = p->R;            //set red as initial max value
    if (p->G > max_val) {max_val = p->G;}   //if green is larger, set new max
    if (p->B > max_val) {max_val = p->B;}   //if blue is larger, set new max
    return max_val;
}

/*******************************************************************************
 * Function to return minimum value within RGB
*******************************************************************************/
unsigned int minRGB(struct RGBC_val *p)
{
    unsigned int min_val = p->R;            //set red as initial min value
    if (p->G < min_val) {min_val = p->G;}   //if green is smaller, set new min
    if (p->B < min_val) {min_val = p->B;}   //is blue is smaller, set new min
    return min_val;
}

/*******************************************************************************
 * Function to scale and adjust values within RGB
*******************************************************************************/
void scaleRGB(struct RGBC_val *p)
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
void getHSVval(struct HSV_val *p1,struct RGBC_val *p2)
{
    unsigned int hue = 0;                   //hue value
    unsigned int sat = 0;                   //saturation value
    unsigned int hue_scale = 60;            //scaling factor for hue
    unsigned int sat_scale = 100;           //scaling factor for saturation
    
    unsigned int M = maxRGB(p2);            //maximum value within RGB
    unsigned int m = minRGB(p2);            //minimum value within RGB
    unsigned int C = M - m;                 //range of RGB (maximum - minimum)
    
    if (C == 0) {                   //C = 0 when grey scale
        hue = 0;                    //h = 0 is arbitrary, prevents zero division
    } else {
        /***************************************************************
         * For each of the six cases below, the commented line shows the
         * actual equation to calculate the hue value
         * 
         * The lines of code underneath is written to calculate the hue
         * value using a temporary long integer to prevent overflow bugs
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
    
    if (M == 0) {                       //M = 0 when colour is completely black
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
    p1->V = M;                              //assign value = maximum RGB value
}

/*******************************************************************************
 * Function that matches RGB values to a colour
 * It takes in a pointer that points to the RGBC_val 
 * The function compares the reading to a set of upperbound and lowerbound readings in this function
 * the upperbound and lowerbound readings are + or - 15% of the recorded reading from 1114-777.
 * Then if it matches any of th upperbound and lowerbound readings then it outputs a colour that the reading corresponds to
 * NOTE: the colour is outputted as a number from 1 - 8 as shown in the pick-card function above 
*******************************************************************************/
unsigned char RGBC2key(struct RGBC_val *p)
{
    unsigned char key = 0;
    if (((p->R >= 2850)&&(p->R <= 3855)) && ((p->G >= 602)&&(p->G <= 814)) && ((p->B >= 1144)&&(p->B <= 1548))) { //red
        key = 1;
    }
    /*
    if (((p->R >= 1630)&&(p->R <= 2205))&&((p->G >= 2604)&&(p->G <= 3523))&&((p->B >= 1851)&&(p->B <= 2505))) {
        2
    }


    if (((p->R >= 537)&&(p->R <= 727))&&((p->G >= 781)&&(p->G <= 1057))&&((p->B >= 1071)&&(p->B <= 1449))) {
        3
    }


    if ((()&&())&&(()&&())&&(()&&())) {

    }


    if ((()&&())&&(()&&())&&(()&&())) {

    }


    if ((()&&())&&(()&&())&&(()&&())) {

    }


    if ((()&&())&&(()&&())&&(()&&())) {

    }


    if ((()&&())&&(()&&())&&(()&&())) {

    }


    if ((()&&())&&(()&&())&&(()&&())) {

    }
     */
    return key;
}