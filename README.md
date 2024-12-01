[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/c8ng1gdc)
# Course project - Mine navigation search and rescue

### Contents
1. Overall Program Flow
2. Colour reading and manoeuvre functions
3. Backtracking functions
4. Different Test Modes
5. Test Maze
6. Challenge brief


## 1. Overall Program Flow

The overall program flow is as the following: 

![Drawing3 (2)](https://github.com/ME3-HECM/final-project-gorakh-boris/assets/77344223/d994f559-40c7-46be-b57f-13373ad3dadd)


Everything inside the while returning is 0 loop is inside the forward_navigation function. Once returning is 0, this function is no longer active and moves to the return_to_sender function which includes the rest of the program flow when returning is set to 1.

To navigate back to the start point, we use 3 sets of arrays: trail_manoeuvre, trail_timer_low, trail_timer_high. trail_manoeuvre stores every instruction that the buggy reads and executes from the colour cards (which it will then do a reverse version of when returning) and the trail_timer_low and trail_timer_high functions stores the timer count for the duration of time the buggy is moving forward between manoeuvres. 

When returing, before the buggy moves forward the timer values is set to the max (11111111, 11111111) minus the stored timer values in the timer_trail arrays. This is done so that the time it takes to interrupt matches the stored length of time that it took to move forward when it was not re-navigating to the start. Then the timer starts and the buggy moves forward. 

We only use one ISR and it is only triggered when the timer interrupts. The timer interrupt is used for two cases: 
1) returning is 0

When returning is 0, the buggy is executing the forward_navigation code. As a result, 
the interrupt will be due to the buggy moving forward for a time longer than roughly 67 
seconds. This means that the buggy is no longer able to store the correct time for 
moving forward. As a result, if this happens, then the lost flag is raised to 1.


2) returning is 1

Here, the interrupt is used to set the return_flag to 1 which then causes the buggy to 
stop and reset the whole return_to_sender loop.


## 2. Colour reading and manoeuvre functions

### 2.1 Colour calibration
To read the colour card given to us, first we read the values from the TCS colour sensor which gave a 16-bit number for red, green, blue and clear colour values of whatever the sensor the reads and sent that data over serial to the computer. The data from serial was stored and the RGB values (still in 16-bit ie values ranging from 0 - 65535) for the different colours were stored. However, we realised that as we moved the buggy slightly away from the colour cards then the values would drop noticeably). 

We then decided to convert RGB to HSV (Hue, Saturation and Value). This was done using the equation obtained from:

https://en.wikipedia.org/wiki/HSL_and_HSV#:~:text=HSV%20stands%20for%20hue%2C%20saturation,hue%2C%20saturation%2C%20and%20intensity.

![image](https://github.com/ME3-HECM/final-project-gorakh-boris/assets/77344223/5e3f86ec-053b-4f5e-ae05-4f48c2e2517e)

![image](https://github.com/ME3-HECM/final-project-gorakh-boris/assets/77344223/d5e32713-ff8c-423d-b5cd-e0a260d1dd99)

![image](https://github.com/ME3-HECM/final-project-gorakh-boris/assets/77344223/53334b98-8d74-4356-bf61-c75b0d1aa18b)


First we determined M and m using our min and max functions. Then we determined C as shown below. Since the equations to determine Hue and saturation results in fractions, we decided to scale our values up using hue_scale and sat_scale.

    unsigned int hue = 0;                   //hue value
    unsigned int sat = 0;                   //saturation value
    unsigned int hue_scale = 60;            //scaling factor for hue
    unsigned int sat_scale = 100;           //scaling factor for saturation
    
    unsigned int M = max_RGB(p2);            //maximum value within RGB
    unsigned int m = min_RGB(p2);            //minimum value within RGB
    unsigned int C = M - m;                 //range of RGB (maximum - minimum)


Furthermore, we realised that since we were not scaling down our RGBC values which was in 16bit, if we multiplied it by our scale, it would overflow and so we decided to convert it into long int. Furthermore, the original equations had the potential of dealing with negative numbers. For example if when the max reading was red, blue was > than green. As a result we split the equation into two cases, where blue was > green and where green was > blue and modified the equations accordingly so we didn't deal with negative numbers. An example for determing the hue when red is the greatest reading is shown below.

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


The hue and saturation were found to be less sensitve to distance from the colour card compared to RGB values. We recorded the readings when the buggy was right up to the colour card in both RGBC (Red, Green Blue, Clear) and HSV. Note, the individual reading for RGBC displayed below are an average of 20 readings against the wall. 

![image](https://github.com/ME3-HECM/final-project-gorakh-boris/assets/77344223/7ae27ba0-5294-4924-b961-069c2226bd56)

We then created upperbound and lowerbound threshold hue and saturation values that would be + or - 15% of the value recorded above. We then tested the cards at lengths up to 4cm away from the buggy and tweaked the thresholds so that it would read the right colour. When doing this however, it was noticed that the black and white colour cards where not as accurate on hue and saturation alone and would read as other colour cards. As a result, we decided to use the clear readings along with saturation to help determine white and black. Furthermore, we noticed that the red colour sensor reading was a lot greater and this was due to the red light in the tri-colour LED being much brighter. As a result, in our code we scaled the red reading down by 50% - this ended up helping with colour reading performance. 
This was all done in the colour.h file as the structure for RGBC is stored there.

### 2.2 Determining manoeuvre from colour readings

After getting the readings, depending on the threshold values, a number from 1 to 9 (represents the 9 colours) is returned using the colour_to_key function. This function is also stored in the colour.h file. Then the function pick_card is used to determine which manoeuvre to execute after reading the colour values and comparing it to the thresholds, this function is stored in the manoeuvres.h file. 
The manoeuvres for the different colour cards are also stored in the manoeuvres.h file.

An example for the manoeuvre code for red is shown below: 

	void card_red(DC_motor *mL, DC_motor *mR, unsigned char backtrack)
	{
    	switch (backtrack) {
        	case 0:         //forward operation
            	headbuttReverse(mL, mR);
            	turnRight90(mL, mR);
            	break;
        	case 1:         //backtracking operation
            	turnLeft90(mL, mR);
            	headbuttReverse(mL, mR);
            	break;
		}
	}

The inputs DC_motor *mL, DC_motor *mR will be the pointers that point to the DC motor structure which will be used to set the correct motor settings for the turns. The input backtrack is used to determine if the buggy is backtracking. If it is backtracking then the backward operation of that instruction will be performed. If it isn't then the forward operation will be performed.


## 3. Backtracking functions

	//temporary timer and manoeuvre variables
        unsigned char timerH = 0;
        unsigned char timerL = 0;
        unsigned char mann = 0;
        
        //read variables from memory
        read_trail(&timerH, &timerL, &mann);

The backtracking function first reads timer high and low, and a manoeuvre variable from the memory trail. The read_trail function moves the array pointers back 1 slot, then reads the values being pointed to and writes it to the input addresses. The read_trail function also decrements the manoeuvre_count counter by 1, which is used to end the entire backtracking function when it reaches 0.

	//perform manoeuvre but ignore white card instruction
        if (mann != 8) {
            pick_card(mL, mR, returning, mann);
        }

The manoeuvre key 8 is written to memory at the end of the forward_navigation function. The U-turn associated with the white card only needs to be performed once, and is already performed at the end of forward_navigation. Therefore, this if statements prevents duplication of the U turn.

	//perform manoeuvre but ignore white card instruction
        if (mann != 8) {
            pick_card(mL, mR, returning, mann);
        }
	
	//toggle main beam for road safety
        toggle_main_beam();
        
        //write timer, start timer and drive forwards
        write_timer(0b11111111 - timerH, 0b11111111 - timerL);
        start_timer();
        fullSpeedAhead(mL, mR);
        
        //wait until timer overflow raises return flag
        while (!return_flag);
        
        //stop driving, stop timer and lower return flag
        stop(mL, mR);
        stop_timer();
        return_flag = 0;
        
        //toggle main beam for road safety
        toggle_main_beam();

The backtracking function first performs the turns associated with the stored card colours, then drives in a straight line for a "remembered" duration. The remembered duration is timed using Timer0 and its overflow interrupt feature: The timer registers are written such that the timer overflows when the time elapsed correlates to timerH and timerL. The timer starts when the car begins driving forward, then when the timer overflows and the return_flag is raised, the car stops.

## 4. Test Cases

In our code we have 4 test cases which are: 

	#define DRIVE 1                     //normal operation, not testing anything
	#define COLOUR_TEST 0               //testing code and sending to serial
	#define MOTOR_TEST 0                //calibrating motor forward and turns
	#define TIMER_TEST 0                //testing timer and memory functions

DRIVE 1 is the normal operation and here the functions forward_navigation and return_to_sender are called only.
COLOUR_TEST is used to send the RGBC and HSV values over serial to the computer.
MOTOR_TEST is used to calibrate the buggy's turns and forward movement.
TIMER_TEST is used to test the recorded timer values for the backtracking function. The way this test is performed is we make the buggy go forward for a predetermined time and we store the timer readings (and display on to the computer via serial). Then the buggy stops, and we rotate the buggy (manually) and then see if it goes back to the starting position.


## 5. Test Maze 

    
https://github.com/ME3-HECM/final-project-gorakh-boris/assets/77344223/b01a8ea1-db25-41db-b1b5-26119c3499cf


Here, we have added a video showing our buggy perform in a test maze. We had to nudge the buggy a few times because our turns were calibrated to a different surface than this one. But it can be observed that the buggy can read the colours and perform the right manoeuvres as well as performing the renavigation back to the start position.

## 6. Challenge brief

Your task is to develop an autonomous robot that can navigate a "mine" using a series of instructions coded in coloured cards and return to its starting position.  Your robot must be able to perform the following: 

1. Navigate towards a coloured card and stop before impacting the card
1. Read the card colour
1. Interpret the card colour using a predefined code and perform the navigation instruction
1. When the final card is reached, navigate back to the starting position
1. Handle exceptions and return back to the starting position if final card cannot be found

## "Mine" environment specification

A "mine" is contstructed from black plywood walls 100mm high with some walls having coloured cards located on the sides of the maze to assist with navigation. The following colour code is to be used for navigation:

Colour | Instruction
---------|---------
Red | Turn Right 90
Green | Turn Left 90
Blue | Turn 180
Yellow | Reverse 1 square and turn right 90
Pink | Reverse 1 square and turn left 90
Orange | Turn Right 135
Light blue | Turn Left 135 
White | Finish (return home)
Black | Maze wall colour

Mine courses will vary in difficulty, with the simplest requiring 4 basic moves to navigate. More advanced courses may require 10 or moves to navigate. The mines may have features such as dead ends but colour cards will always direct you to the end of the maze. Once the end of the maze has been reached, you must return to the starting position. An example course to navigate is shown below. You do not know in advance which colours will be in the course or how many.

![Navi Diagram](gifs/maze.gif)

## Resources and project submission

To develop your solution you have your Clicker 2 board, buggy and colour click add on board. You must not use any hardware that is not provided as part of this course and you must use the XC8 compiler to compile your C code. 

Please use this GitHub repo to manage your software development and submit your project code. 

Final testing will take place in the CAGB foyer and testing areas will be provided around the mechatronics lab. You will not know the exact layout of the maze(s) in advance. You will also be asked to give a short presentation on the testing day.

## Supplementary technical information

### Additional buggy features

In addition to the motor drives we explored in lab 6, the buggy contains some additional features that may be of use during the project. The first feature is additional LEDs, controlled through the pins labelled **H.LAMPS**, **M.BEAM**, **BRAKE**, **TURN-L** and **TURN-R**. H.LAMPS turns on the front white LEDs and rear red LEDs, at a reduced brightness. M.BEAM and BRAKE enable you to turn these LEDs on at full brightness. The turn signals have not hardware based brightness control. These LEDs give you a method to provide feedback for debugging, in addition of the LEDs on the clicker board.

![Buggy pinout](gifs/buggy_pins.png)

A further feature of the buggy is **BAT-VSENSE** pin which allows you to monitor the batter voltage via an analogue input pin. The battery is connected across a voltage divider, as shown in the diagram below:

![Bat sense](gifs/bat_vsense.png)

The voltage at BAT-VSENSE will always be one third of that at the battery. Measuring this value can be useful for determining if your battery needs charging. You could also use it to calibrate your robot to perform well at different charge levels. 

### Colour click

The Colour click board contains 2 devices to help with navigation, a tri-colour LED for illumination and a 4 channel RGBC photodiode sensor. This combination of two devices (an illumination source and a sensor) allow you a make measurements of the reflected colour of objects near the sensor. The circuit diagram for the Colour click is shown below:

![Color Cick](gifs/color_click.png)

The tri-colour LED is the simpler of the two devices to control. Three separate pins control the red, green and blue LEDs individually, despite them being in a single package. Control via these pins is simple digital on/off control and if any brightness control was required, the user would need program the microcontroller to generate a PWM signal to achieve this.  

The second device on the Colour click is the TCS3471 colour light-to-digital converter. The sensor contains a grid of 4x4 photodiodes, 4 are sensitive to red light, 4 green light, 4 blue light and 4 "clear" light (that is, a range of wavelengths, see datasheet for exact spectral response). When light falls on the photodiode the photons are absorbed and current is generated. This signal is then integrated over time using integrators and sampled by 16 bit on board ADCs. Communication with the device is achieved using an I2C interface. This enables configuration of the device to customise sampling of the data (i.e. integration time, gain, etc.) and to read the 16 bit digital values for each of the RGBC channels. The relative magnitude of these values gives you information about the colour of light that is falling on the sensor. The device can also be configured to send an interrupt signal to the PIC when signal reaches a preset value.

### I2C

The I2C interface widely used in industry for communication between microcontrollers and peripheral integrated circuits (other chips) over short distances. I2C is serial communication bus that enables communication between many devices over a simple 2 wire interface. One wire is the data line (SDA) and is used for both transmission and receiving. The second wire (SCL) is used for a clock signal to ensure all devices are synchronous. To ensure communication of data occurs without problem a message protocol must be followed and understood by all devices on the bus. Devices are termed master and slave devices, with master devices initiation communication to a slave device via unique address for that device. The general sequence of communication between a master/slave over the I2C interface is a follows:

1. Send a Start bit
1. Send the slave address, usually 7 bits
1. Send a Read (1) or Write (0) bit to define what type of transaction it is
1. Wait for an Acknowledge bit
1. Send a data or command byte (8 bits)
1. Wait for Acknowledge bit
1. Send the Stop bit

This is shown pictorial in the figure below:

![I2C](gifs/i2c.png)

Although it is possible to program an entirely software based I2C interface, luckily for us our PIC chip has a module dedicated to generating and receiving I2C signals: the Master Synchronous Serial Port Module, or MSSP (see chapter 28 of the PIC datasheet). This module provides methods for sending start/stop/acknowledge bits and allows us to focus on sending/receiving data.

The included i2c.c/h files contain functions to help you get started with I2C communication. The first function below sets up the MSSP module as an I2C master device and configures the necessary pins.

	void I2C_2_Master_Init(void)
	{
		//i2c config  
		SSP2CON1bits.SSPM= 0b1000;    // i2c master mode
		SSP2CON1bits.SSPEN = 1;       //enable i2c
		SSP2ADD = (_XTAL_FREQ/(4*_I2C_CLOCK))-1; //Baud rate divider bits (in master mode)
  
		//pin configuration for i2c  
		TRISDbits.TRISD5 = 1;                   //Disable output driver
		TRISDbits.TRISD6 = 1;                   //Disable output driver
		ANSELDbits.ANSELD5=0;					// disable analogue on pins
		ANSELDbits.ANSELD6=0;					// disable analogue on pins
		SSP2DATPPS=0x1D;      //pin RD5
		SSP2CLKPPS=0x1E;      //pin RD6
		RD5PPS=0x1C;      // data output
		RD6PPS=0x1B;      //clock output
	}
	
Bits with the SSP2CON2 register are set to send the individual start/stop/acknowledge bits used in the protocol. These must only be set when the bus is idle (nothing being sent/received). The I2C_2_Master_Start(), I2C_2_Master_Stop() and I2C_2_Master_RepStart() functions allow you add the necessary bits as defined in the protocol above. Data is sent on the bus using the SSP2BUF register:

	void I2C_2_Master_Write(unsigned char data_byte)
	{
		I2C_2_Master_Idle();
		SSP2BUF = data_byte;         //Write data to SSPBUF
	}

Data is also read using the same SSP2BUF registers. However, to receive data we first need to switch the module into receiver mode. We also need to start the acknowledge sequence to let the slave device know what we have received the data OK. The following function achieves this:

	unsigned char I2C_2_Master_Read(unsigned char ack)
	{
		unsigned char tmp;
		I2C_2_Master_Idle();
		SSP2CON2bits.RCEN = 1;        // put the module into receive mode
		I2C_2_Master_Idle();
		tmp = SSP2BUF;                //Read data from SS2PBUF
		I2C_2_Master_Idle();
		SSP2CON2bits.ACKDT = !ack;     // 0 turns on acknowledge data bit
		SSP2CON2bits.ACKEN = 1;        //start acknowledge sequence
		return tmp;
	}

The functions described so form the basics required for I2C communication with the PIC. To communicate with the TCS3471 onboard the Colour click we first need to know its address. This is listed in the data sheet as 0x29. To send our first byte over the I2C we need to send this address combined with the transaction type (read or write) as defined in the protocol diagram above. This lets the TCS3471 know the message is intended for it and not some other device on the interface. Next we send a byte which is a combination of command type and the register address in the TCS3471 that we want to write to. Finally we the value that we want to write to that register. This sequence is shown in the function below:

	void color_writetoaddr(char address, char value){
		I2C_2_Master_Start();         		//Start condition
		I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write (0) mode (note 0x52=0x29<<1)
		I2C_2_Master_Write(0x80 | address);    //command + register address
		I2C_2_Master_Write(value);    			//value to store in the register
		I2C_2_Master_Stop();          //Stop condition
	}

We then call the function to, for example, turn the device on:

	color_writetoaddr(0x00, 0x01); // write 1 to the PON bit in the device enable register
	
There are additional commands that must be set to initialise the device and many registers that be configured to obtain optimal performance for the sensor in your conditions. It is up to you to carefully read the TCS3471 datasheet and experiment with this.

To read values from the TCS3471 you need to a similar sequence to above but you first need to tell the device which register you want to deal with, before telling the device you want read from it. The example below uses the combined read format to read multiple bytes in sequence. The auto-increment is set so that instead of reading the same register over and over again, it automatically advances to the next one. The example starts at the Red channel low byte address and then automatically advances and reads the associated high byte.

	unsigned int color_read_Red(void)
	{
		unsigned int tmp;
		I2C_2_Master_Start();         //Start condition
		I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
		I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
		I2C_2_Master_RepStart();
		I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
		tmp=I2C_2_Master_Read(1);			// read the Red LSB
		tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
		I2C_2_Master_Stop();          //Stop condition
		return tmp;
	}

Instead of separate functions for each channel you may want to create a structure to store all the values together, and pass a pointer to the function so that all values in the structure can be updated in one operation. An example structure might look like this:

	//definition of RGB structure
	struct RGB_val { 
		unsigned int R;
		unsigned int G;
		unsigned int B;
	};

This concludes the basics of I2C and communication with the colour sensor. Best of luck! 


## 2. Overall program flow
