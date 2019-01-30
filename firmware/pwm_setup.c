#include "pwm_setup.h"
#include <p30Fxxxx.h> //picks the correct device automatically

void pwm_setup()
{

    /* PWM Period in Up/Down Counting Modes
     * PTPER = Fcy / (Fpwm*Prescaler*2) - 1
     * Fcy = 20Mhz
     * Fpwm = 20khz
     * PTPER = 499
     */
    
    PTCONbits.PTEN = 0;
    PTCONbits.PTCKPS = 0;
    PTPER = 499;
    
    // PWM 1
    PWMCON1bits.PMOD1 = 1;
    PWMCON1bits.PEN1L = 1;
    PWMCON1bits.PEN1H = 0;
    
    // PWM 2
    PWMCON1bits.PMOD2 = 1;
    PWMCON1bits.PEN2L = 0;
    PWMCON1bits.PEN2H = 0;

    // PWM 3
    PWMCON1bits.PMOD3 = 1;
    PWMCON1bits.PEN3L = 0;
    PWMCON1bits.PEN3H = 0;

    PTCONbits.PTEN = 1;
}
