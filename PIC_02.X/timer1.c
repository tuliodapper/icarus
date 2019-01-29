// PIC 02

#include <p30Fxxxx.h> //picks the correct device automatically
#include "timer1.h"
#include "../velocity_control.h"
#include "../timer1_setup.h"
#include "../pwm_setup.h"
#include "io.h"
#include <stdio.h>

static unsigned int dead_zone = 100;

void setDeadZone(unsigned int value){
    dead_zone = value;
}

void T1interrupt_10ms(){

    int velocity;
    int control_value;

    velocity = POSCNT - 0x7fff;
    POSCNT = 0x7fff;

    control_value = PID_controller(velocity);

    if (control_value > 0)
    {
        DIRALatch = 0;
        DIRBLatch = 1;
    } else
    {
        DIRALatch = 1;
        DIRBLatch = 0;
        control_value = -control_value;
    }

    if (control_value > 1000) PWM_DC = 1000;
    if (control_value < dead_zone) PWM_DC = 0;
    else PWM_DC = control_value;
    
}
