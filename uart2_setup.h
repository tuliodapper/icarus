/*
 * File:   uart2_setup.h
 * Author: TúlioDapper
 *
 * Created on 12 January 2015, 17:50
 */

#ifndef UART2_SETUP_H
#define	UART2_SETUP_H

void uart2_setup(unsigned long int baud);
void uart2_mySendString(char* stringTMP);
void uart2_sendParameter(char c, int value);
void uart2_changeBaudRate(unsigned long int baud);
void uart2_sendEnter(void);


#endif	/* UART2_SETUP_H */