/*
 * File:   uart.h
 * Author: TúlioDapper
 *
 * Created on 12 January 2015, 17:50
 */

#ifndef UART_FUNCTIONS_H
#define	UART_FUNCTIONS_H

#define _UART_1 0
#define _UART_2 1

#define BAUDRATE_BLUETOOTH_CONFIGURATION_MODE 57600

#define BAUDRATE_BLUETOOTH 57600
#define BAUDRATE_PIC 1250000

void rcvHandler (char c, int way);
void uart_receive(char cmd, int value, int way);
int getErrorCounter(void);
void resetErrorCounter(void);
void incrementErrorCounter(void);
void decrementErrorCounter(void);


#endif	/* UART_FUNCTIONS_H */