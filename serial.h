#ifndef _SERIAL_H
#define _SERIAL_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


#define TX_BUF_SIZE 60


/* TODO: serial.c/h are for testing only, remove before final build
 *
 */


volatile char EUSART4TXbuf[TX_BUF_SIZE];
volatile char TxBufWriteCnt=0;
volatile char TxBufReadCnt=0;


//basic EUSART functions
void initUSART4(void);
void sendCharSerial4(char charToSend);
void sendStringSerial4(char *string);



// circular Tx buffer functions (Ex3+)
char getCharFromTxBuf(void);
void putCharToTxBuf(char byte);
char isDataInTxBuf (void);
void TxBufferedString(char *string); //Send buffered string with interrupts
void sendTxBuf(void);

#endif

