#include <xc.h>
#include <stdio.h>
#include "serial.h"

/*******************************************************************************
 * Function to set up USART4 for serial communication
*******************************************************************************/
void initUSART4(void) {
    RC0PPS = 0x12;              //map EUSART4 TX to RC0
    RX4PPS = 0x11;              //RX is RC1 
    
    BAUD4CONbits.BRG16 = 0; 	//set baud rate scaling
    TX4STAbits.BRGH = 0; 		//high baud rate select bit
    SP4BRGL = 51;               //set baud rate to 51 = 19200bps
    SP4BRGH = 0;                //not used

    RC4STAbits.CREN = 1; 		//enable continuos reception
    TX4STAbits.TXEN = 1; 		//enable transmitter
    RC4STAbits.SPEN = 1; 		//enable serial port
}

/*******************************************************************************
 * Function to wait for a byte to arrive on serial port and read it once it does
*******************************************************************************/
char getCharSerial4(void) {
    while (!PIR4bits.RC4IF);            //wait for the data to arrive
    return RC4REG;                      //return byte in RCREG
}

/*******************************************************************************
 * Function to check the TX reg is free and send a byte
*******************************************************************************/
void sendCharSerial4(char charToSend) {
    while (!PIR4bits.TX4IF);            //wait for flag to be set
    TX4REG = charToSend;                //transfer char to transmitter
}

/*******************************************************************************
 * Function to send a string over the serial interface
*******************************************************************************/
void sendStringSerial4(char *string) {
	//Hint: look at how you did this for the LCD lab 
    while(*string != 0){                //while the data pointed to isn't a 0x00
		sendCharSerial4(*string++); 	//Send out the current byte pointed to and increment the pointer
	}
}

/*******************************************************************************
 * Function to send an integer over the serial interface
*******************************************************************************/
void sendIntSerial4(int integer) {
    //https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-itoa-convert-int-into-string
    char string[sizeof(int) * 8 + 1];
    sprintf(string, "%d \r", integer);
    sendStringSerial4(string);
}

/*******************************************************************************
 * Function to send an array of unsigned characters over the serial interface
*******************************************************************************/
void sendArrayCharSerial4(unsigned char *arr) {
    //https://stackoverflow.com/questions/30234363/how-can-i-convert-an-int-array-into-a-string-array
    unsigned char index = 0;
    char tempStr[20 * 8 + 1];           //20 is the length of the input array
    for (unsigned int i = 0; i < 20; i++) {
        index += sprintf(&tempStr[index], "%d ", arr[i]);
    }
    sendStringSerial4(tempStr);
    sendStringSerial4(" \r");           //new line and left justify
}

/*******************************************************************************
 * Function to send a RGBC_val structure over the serial interface
*******************************************************************************/
void sendRGBCvalSerial4(RGBC_val *col_val) {
    char tempStr[26];
    //use %u for unsigned integer https://w3resource.com/c-programming/stdio/c_library_method_sprintf.php
    sprintf(tempStr, "%u %u %u %u \r", col_val->R, col_val->G, col_val->B, col_val->C);
    sendStringSerial4(tempStr);
}

/*******************************************************************************
 * Function to send a HSV_val structure over the serial interface
*******************************************************************************/
void sendHSVvalSerial4(HSV_val *col_val) {
    char tempStr[21];
    //use %u for unsigned integer https://w3resource.com/c-programming/stdio/c_library_method_sprintf.php
    sprintf(tempStr, "%u %u %u \r", col_val->H, col_val->S, col_val->V);
    sendStringSerial4(tempStr);
}