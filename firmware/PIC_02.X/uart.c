// UART PIC_02
#include "../uart_functions.h"
#include "../uart1_setup.h"
#include "../velocity_control.h"
#include "io.h"
#include "timer1.h"

static unsigned int flag_config = 0;

unsigned int get_UART_flag_config(){
    return flag_config;
}

void uart_receive(char cmd, int value, int way){
    switch (cmd) {
        case 'P': //PWM set
                setKp(value);
                uart1_sendParameter('O',value);
            break;
        case 'I': //PWM set
                setKi(value);
                uart1_sendParameter('O',value);
            break;
        case 'D': //PWM set
                setKd(value);
                uart1_sendParameter('O',value);
            break;
        case 'z': //PWM set
                setDeadZone(value);
            break;
        case 'V': //PWM set
                setDesiredVelocity(value);
            break;
        case 'T': //PWM set
                toggleLED(value);
            break;
        case 'S': //PWM set
                setLED(value);
            break;
        case 'R': //PWM set
                resetLED(value);
            break;
    }
}