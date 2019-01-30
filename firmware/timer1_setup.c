#include <p30Fxxxx.h> //picks the correct device automatically
#include "timer1_setup.h"
#include "uart.h"
#include "PIC_01.X/uart.h"

void timer1_setup()
{

    /*  TIMER 1 CONFIGURATION
     * PR1 = (Fcy * Ts)/prescaler - 1
     * Fcy = 20Mhz
     * Ts = 10ms
     * prescaler = 8
     * PR1 = 24999
     */
    
    T1CONbits.TON       = 0; //
    TMR1                = 0; // Resets the timer to 0
    T1CONbits.TCKPS     = 1; // Pre-scale value 8
    PR1                 = 24999; // Period register to achieve 10ms
    T1CONbits.TCS       = 0; // Choose internal clock or not
    T1CONbits.TSIDL     = 1; // Stop timer in idle mode
    T1CONbits.TGATE     = 0; // Gate the timer or nor
    IFS0bits.T1IF       = 0; // Clear interrupt flag
    IPC0bits.T1IP       = 3; // Choose interrupt priority equal to 3
    IEC0bits.T1IE       = 1; // Enable Timer1 interrupt
    T1CONbits.TON       = 1; // Switches the Timer1 ON.
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0; ///reset the timer 1 interrupt flag
  //  if (getUART_CONFIG() == 0)
    T1interrupt_10ms();
}

