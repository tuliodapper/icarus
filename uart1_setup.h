/*
 * File:   uart1_setup.h
 * Author: TúlioDapper
 *
 * Created on 12 January 2015, 17:50
 */

#ifndef UART1_SETUP_H
#define	UART1_SETUP_H

void uart1_setup(unsigned long int baud);
void uart1_mySendString(char* stringTMP);
void uart1_sendParameter(char c, int value);
void uart1_changeBaudRate(unsigned long int baud);
void uart1_sendEnter(void);

#endif	/* UART1_SETUP_H */