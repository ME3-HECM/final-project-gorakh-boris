#include <xc.h>
#include <stdio.h>
#include "serial.h"

void initUSART4(void) {

	//code to set up USART4 for Reception and Transmission =
	//see readme for detials
    RC0PPS = 0x12; // Map EUSART4 TX to RC0
    RX4PPS = 0x11; // RX is RC1 
    
    BAUD4CONbits.BRG16 = 0; 	//set baud rate scaling
    TX4STAbits.BRGH = 0; 		//high baud rate select bit
    SP4BRGL = 51; 			//set baud rate to 51 = 19200bps
    SP4BRGH = 0;			//not used

    RC4STAbits.CREN = 1; 		//enable continuos reception
    TX4STAbits.TXEN = 1; 		//enable transmitter
    RC4STAbits.SPEN = 1; 		//enable serial port
}

//function to wait for a byte to arrive on serial port and read it once it does 
char getCharSerial4(void) {
    while (!PIR4bits.RC4IF);//wait for the data to arrive
    return RC4REG; //return byte in RCREG
}

//function to check the TX reg is free and send a byte
void sendCharSerial4(char charToSend) {
    while (!PIR4bits.TX4IF); // wait for flag to be set
    TX4REG = charToSend; //transfer char to transmitter
}

//function to send a string over the serial interface
void sendStringSerial4(char *string) {
	//Hint: look at how you did this for the LCD lab 
    while(*string != 0){  // While the data pointed to isn?t a 0x00 do below (strings in C must end with a NULL byte) 
		sendCharSerial4(*string++); 	//Send out the current byte pointed to and increment the pointer
	}
}

//function to send an integer over the serial interface
void sendIntSerial4(int integer) {
    char string[sizeof(int) * 8 + 1]; //Referenced from https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-itoa-convert-int-into-string
    sprintf(string, "%d \r", integer);
    sendStringSerial4(string);
}

//function to send an array of unsigned characters over the serial interface
void sendArrayCharSerial4(unsigned char *arr) {
    unsigned char index = 0;
    //20 is the length of the input array
    char tempStr[20 * 8 + 1];
    for (unsigned int i = 0; i < 20; i++) {     //Referenced from https://stackoverflow.com/questions/30234363/how-can-i-convert-an-int-array-into-a-string-array
        index += sprintf(&tempStr[index], "%d ", arr[i]);
    }
    sendStringSerial4(tempStr);
    sendStringSerial4(" \r");
}

//function to send a RGBC_val structure over the serial interface
void sendRGBCvalSerial4(RGBC_val *col_val) {
    char tempStr[26];
    //use %u for unsigned integer https://w3resource.com/c-programming/stdio/c_library_method_sprintf.php
    sprintf(tempStr, "%u %u %u %u \r", col_val->R, col_val->G, col_val->B, col_val->C);
    sendStringSerial4(tempStr);
}

//function to send a HSV_val structure over the serial interface
void sendHSVvalSerial4(HSV_val *col_val) {
    char tempStr[21];
    //use %u for unsigned integer https://w3resource.com/c-programming/stdio/c_library_method_sprintf.php
    sprintf(tempStr, "%u %u %u \r", col_val->H, col_val->S, col_val->V);
    sendStringSerial4(tempStr);
}

//functions below are for Ex3 and 4 (optional)

// circular buffer functions for RX
// retrieve a byte from the buffer
char getCharFromRxBuf(void){
    if (RxBufReadCnt>=RX_BUF_SIZE) {RxBufReadCnt=0;} 
    return EUSART4RXbuf[RxBufReadCnt++];
}

// add a byte to the buffer
void putCharToRxBuf(char byte){
    if (RxBufWriteCnt>=RX_BUF_SIZE) {RxBufWriteCnt=0;}
    EUSART4RXbuf[RxBufWriteCnt++]=byte;
}

// function to check if there is data in the RX buffer
// 1: there is data in the buffer
// 0: nothing in the buffer
char isDataInRxBuf (void){
    return (RxBufWriteCnt!=RxBufReadCnt);
}

// circular buffer functions for TX
// retrieve a byte from the buffer
char getCharFromTxBuf(void){
    if (TxBufReadCnt>=TX_BUF_SIZE) {TxBufReadCnt=0;} 
    return EUSART4TXbuf[TxBufReadCnt++];
}

// add a byte to the buffer
void putCharToTxBuf(char byte){
    if (TxBufWriteCnt>=TX_BUF_SIZE) {TxBufWriteCnt=0;}
    EUSART4TXbuf[TxBufWriteCnt++]=byte;
}

// function to check if there is data in the TX buffer
// 1: there is data in the buffer
// 0: nothing in the buffer
char isDataInTxBuf (void){
    return (TxBufWriteCnt!=TxBufReadCnt);
}

//add a string to the buffer
void TxBufferedString(char *string){
	//Hint: look at how you did this for the LCD lab 
    while(*string != 0) {
        putCharToTxBuf(*string++);
    }
}

//initialise interrupt driven transmission of the Tx buf
//your ISR needs to be setup to turn this interrupt off once the buffer is empty
void sendTxBuf(void){
    if (isDataInTxBuf()) {PIE4bits.TX4IE=1;} //enable the TX interrupt to send data
}