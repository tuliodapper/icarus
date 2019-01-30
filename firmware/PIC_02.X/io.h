    /*
 * File:   ioSetup.h
 * Author: t2-dapperesilv
 *
 * Created on 15 October 2014, 11:29
 */

#ifndef IOSETUP_H
#define	IOSETUP_H

#include <p30Fxxxx.h> //picks the correct device automatically

#define BUTTONTris TRISDbits.TRISD1
#define BUTTONLatch LATDbits.LATD1
#define BUTTONPort !PORTDbits.RD1

#define LED3Tris TRISDbits.TRISD0
#define LED3Latch LATDbits.LATD0
#define LED3Port PORTDbits.RD0

#define LED4Tris TRISEbits.TRISE8
#define LED4Latch LATEbits.LATE8
#define LED4Port PORTEbits.RE8

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

void io_setup();
void setLED(unsigned int LED);
void resetLED(unsigned int LED);
void toggleLED(unsigned int LED);
void read_buttons();
void reset_buttons();
unsigned int get_old_value_s3();
unsigned int get_current_value_s3();


#endif	/* IOSETUP_H */