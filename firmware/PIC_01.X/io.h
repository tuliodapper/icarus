/*
 * File:   ioSetup.h
 * Author: t2-dapperesilv
 *
 * Created on 15 October 2014, 11:29
 */

#ifndef IOSETUP_H
#define	IOSETUP_H

#include <p30Fxxxx.h> //picks the correct device automaticallys

#define BUTTONTris TRISBbits.TRISB0
#define BUTTONLatch LATBbits.LATB0
#define BUTTONPort !PORTBbits.RB0

#define LED1Tris TRISEbits.TRISE3
#define LED1Latch LATEbits.LATE3
#define LED1Port PORTEbits.RE3

#define LED2Tris TRISEbits.TRISE4
#define LED2Latch LATEbits.LATE4
#define LED2Port PORTEbits.RE4

#define PWMTris TRISEbits.TRISE0
#define PWMLatch LATEbits.LATE0
#define PWMPort PORTEbits.RE0

#define DIRATris TRISEbits.TRISE1
#define DIRALatch LATEbits.LATE1
#define DIRAPort PORTEbits.RE1

#define DIRBTris TRISEbits.TRISE2
#define DIRBLatch LATEbits.LATE2
#define DIRBPort PORTEbits.RE2

#define ENCATris TRISBbits.TRISB4
#define ENCALatch LATBbits.LATB4
#define ENCAPort PORTBbits.RB4

#define ENCBTris TRISBbits.TRISB5
#define ENCBLatch LATBbits.LATB5
#define ENCBPort PORTBbits.RB5

#define FRONT_RIGHT_SENSOR_Tris TRISDbits.TRISD2
#define FRONT_RIGHT_SENSOR_Latch LATDbits.LATD2
#define FRONT_RIGHT_SENSOR_Port PORTDbits.RD2

#define FRONT_LEFT_SENSOR_Tris TRISEbits.TRISE8
#define FRONT_LEFT_SENSOR_Latch LATEbits.LATE8
#define FRONT_LEFT_SENSOR_Port PORTEbits.RE8

#define DIAGONAL_RIGHT_SENSOR_Tris TRISFbits.TRISF6
#define DIAGONAL_RIGHT_SENSOR_Latch LATFbits.LATF6
#define DIAGONAL_RIGHT_SENSOR_Port PORTFbits.RF6

#define DIAGONAL_LEFT_SENSOR_Tris TRISDbits.TRISD3
#define DIAGONAL_LEFT_SENSOR_Latch LATDbits.LATD3
#define DIAGONAL_LEFT_SENSOR_Port PORTDbits.RD3

#define RIGHT_SENSOR_Tris TRISDbits.TRISD0
#define RIGHT_SENSOR_Latch LATDbits.LATD0
#define RIGHT_SENSOR_Port PORTDbits.RD0

#define LEFT_SENSOR_Tris TRISDbits.TRISD1
#define LEFT_SENSOR_Latch LATDbits.LATD1
#define LEFT_SENSOR_Port PORTDbits.RD1


void io_setup();
void read_buttons();
void reset_buttons();
unsigned int get_old_value_s3();
unsigned int get_current_value_s3();

#endif	/* IOSETUP_H */