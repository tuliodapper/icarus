/*
 * File:   uart.h
 * Author: TúlioDapper
 *
 * Created on 12 January 2015, 17:50
 */

#ifndef UART_H
#define	UART_H

unsigned int getUARTFlagMove();
unsigned int getUARTDiretion();
unsigned int getUARTCells();
void resetUARTFlagMove();
unsigned int getUARTFlagButton();
unsigned int getUART_MOVING();
unsigned int getUART_MAPPING();
unsigned int getUART_CONFIG();
void setUART_MOVING(unsigned int value);

#endif	/* UART_H */