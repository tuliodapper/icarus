#include <p30Fxxxx.h>
#include "uart2_setup.h"
#include "uart_functions.h"
#include <stdio.h>

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    IFS1bits.U2RXIF = 0; //clear U2 rx interrupt flag
    rcvHandler(U2RXREG, _UART_2);
}
void uart2_setup(unsigned long int baud) {
        U2MODEbits.UARTEN = 0;
        U2BRG = (unsigned int) (1250000 / baud) - 1; //calculate appropriate baud rate
        U2MODEbits.LPBACK = 0;
        U2MODEbits.WAKE = 0;
        U2MODEbits.ABAUD = 0;
        U2MODEbits.PDSEL = 0;
        U2MODEbits.STSEL = 0;
        U2STAbits.URXISEL = 0;
        IFS1bits.U2RXIF = 0;
        IPC6bits.U2RXIP = 4;
        IEC1bits.U2RXIE = 1;
        U2MODEbits.UARTEN = 1;
        U2STAbits.UTXEN =   1;
}

void uart2_changeBaudRate(unsigned long int baud){
	U2BRG = (unsigned int) (1250000 / baud) - 1; //calculate appropriate baud rate
}

void uart2_sendParameter(char c, int value){
    char str[15];
    sprintf(str, "<%c%4d>", c, value);
    uart2_mySendString(str);
}

void uart2_mySendString(char* stringTMP){
        while (*stringTMP) {
            if (!U2STAbits.UTXBF) { // if buffer is NOT full
                U2TXREG = *stringTMP++;
            }
        }
}

void uart2_sendEnter(void){
    uart2_mySendString("\n");
}