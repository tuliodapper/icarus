/* 
 * File:   pwm.h
 * Author: TúlioDapper
 *
 * Created on 14 January 2015, 21:56
 */

#ifndef PWM_H
#define	PWM_H

#include <p30Fxxxx.h> //picks the correct device automatically

#define PWM_DC PDC1

void pwm_setup();

#endif	/* PWM_H */

