// PIC 02

#include <p30Fxxxx.h>
#include "main.h"
#include "timer1.h"
#include "io.h"
#include "../pwm_setup.h"
#include "uart.h"
#include "../uart_functions.h"
#include "../uart1_setup.h"
#include "../velocity_control.h"
#include "../qei.h"

_FOSC(PRI  &  XT_PLL8 & CSW_FSCM_OFF  ); 

_FWDT(WDT_OFF); 
 
_FBORPOR( PBOR_OFF & MCLR_EN & PWRT_64 & PWMxL_ACT_HI &  PWMxH_ACT_LO & RST_PWMPIN);

void myDelay(unsigned int inTimeMiliSeconds)
{
    unsigned int i, j;

    for (i=0;i<inTimeMiliSeconds;i++)
        for (j=0;j<1998;j++);

}

int main(void)
{
    int button = 0;

    io_setup();

    uart1_setup(BAUDRATE_PIC);

    pwm_setup();

    qei_setup();
    
    timer1_setup();

    DIRALatch = 1;
    DIRBLatch = 0;
    
    LED3Latch = 1;

    while(1)
    {
        // Delay aiming to have 1ms between samples.
        myDelay(1);

        // Verify how many samples the button is (un)pressed and the current value can be updated.
        read_buttons();

        // If the current value and the old value are in this configuration, so, act.
        if ((get_old_value_s3() == 1) && (get_current_value_s3() == 0))
        {
            if (button == 0) button = 1; else button = 0;
            // Invert the output.
            uart1_sendParameter('B', button);
        }

        // Makes the old value equal to the current value
        reset_buttons();

    };

        
    return 0;
}