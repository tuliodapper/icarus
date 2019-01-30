#include <p30Fxxxx.h>
#include "uart1_setup.h"
#include "uart_functions.h"
#include <stdio.h>

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0; //clear U1 rx interrupt flag
    rcvHandler(U1RXREG, _UART_1);
}

void uart1_setup(unsigned long int baud) {

        U1MODEbits.UARTEN = 0; //disable UART during config
        U1BRG = (unsigned int) (1250000 / baud) - 1; //calculate appropriate baud rate
        U1MODEbits.LPBACK = 0; //disable loopback mode
        U1MODEbits.WAKE = 0; //do not wake-up on serial activity
        U1MODEbits.ABAUD = 0; //no auto-baud detection
        U1MODEbits.PDSEL = 0; //8 databits, no parity
        U1MODEbits.STSEL = 0; //one stop bit
        U1MODEbits.ALTIO = 1; //alternative transmit and receiver are used by the UART module
        U1STAbits.URXISEL = 0; //interrupts RX Interrupt when 1 character has arrived
        IFS0bits.U1RXIF = 0; //clear U2 rx interrupt flag
        IPC2bits.U1RXIP = 4; //set interrupt priority
        IEC0bits.U1RXIE = 1; //enable interrupt
        U1MODEbits.UARTEN = 1; //enable UART2
        U1STAbits.UTXEN = 1; //enable transmission

}

void uart1_changeBaudRate(unsigned long int baud){
        U1BRG = (unsigned int) (1000000 / baud) - 1; //calculate appropriate baud rate
}

void uart1_sendParameter(char c, int value){
    char str[15];
    sprintf(str, "<%c%4d>", c, value);
    uart1_mySendString(str);
}

void uart1_mySendString(char* stringTMP){
        while (*stringTMP) {
            if (!U1STAbits.UTXBF) { // if buffer is NOT full
                U1TXREG = *stringTMP++;
            }
        }
}

void uart1_sendEnter(void){
    uart1_mySendString("\n");
}