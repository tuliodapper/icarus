// UART PIC_01

#include "../uart2_setup.h"
#include "../uart_functions.h"
#include "../velocity_control.h"
#include "uart.h"
#include "position_control.h"
#include "sensor.h"
#include "timer1.h"

// START OF DELETING
static unsigned int direction = 0;
static unsigned int cells = 0;
static unsigned int flag_move = 0;
static unsigned int flag_config = 0;
static unsigned int flag_button = 0;
static unsigned int const_MOVING = 0;
static unsigned int const_MAPPING = 0;

unsigned int getUARTFlagButton(){
    return flag_button;
}
unsigned int getUART_MOVING(){
    return const_MOVING;
}
unsigned int getUART_CONFIG(){
    return flag_config;
}
void setUART_MOVING(unsigned int value){
    const_MOVING = value;
}
unsigned int getUART_MAPPING(){
    return const_MAPPING;
}

unsigned int getUARTFlagMove(){
    return flag_move;
}
unsigned int getUARTDiretion(){
    return direction;
}
unsigned int getUARTCells(){
    return cells;
}
void resetUARTFlagMove(){
    flag_move = 0;
}

void uart_receive(char cmd, int value, int way){
        switch (cmd) {
            case 'O':
                   decrementErrorCounter();
                break;
            // START OF DELETING
            case 'P':
                   setKp(value);
                   uart2_sendParameter('P', value);
                break;
            case 'I':
                   setKi(value);
                   uart2_sendParameter('I', value);
                break;
            case 'D':
                   setKd(value);
                   uart2_sendParameter('D', value);
                break;
            case 'z':
                   setDeadZone(value);
                   uart2_sendParameter('z', value);
                break;
            case 'F':
                   setDISTANCE_FOWARD(value);
                break;
            case 'o':
                   setDISTANCE_FOWARD_MID_CELL_OUT(value);
                break;
            case 'i':
                   setDISTANCE_FOWARD_MID_CELL_IN(value);
                break;
            case 'f':
                   setDISTANCE_FOWARD_STOP_WALL(value);
                break;
            case 'Y':
                   setDISTANCE_TURN(value);
                break;
            case 'R':
                   setREFERENCE_VELOCITY_FOWARD(value);
                break;
            case 'T':
                   setREFERENCE_VELOCITY_TURN(value);
                break;
            case 'C':
                   setCONSTANT_CORRECTION_VELOCITY(value);
                break;
            case 'Q':
                   setCONSTANT_CORRECTION_VELOCITY_DIVISOR(value);
                break;
            case 'x':
                   setIDEAL_DISTANCE_FRONT(value);
                break;
            case 'X':
                   setIDEAL_DISTANCE_LEFT(value);
                break;
            case 'Z':
                   setIDEAL_DISTANCE_RIGHT(value);
                break;
            case 'V':
                   setTHRESHOLD_FRONT(value);
                break;
            case 'G':
                   setTHRESHOLD_DIAGONAL_LEFT(value);
                break;
            case 'H':
                   setTHRESHOLD_DIAGONAL_RIGHT(value);
                break;
            case 'J':
                   setTHRESHOLD_LEFT(value);
                break;
            case 'K':
                   setTHRESHOLD_RIGHT(value);
                break;
            case 'L':
                   setNUMBER_SAMPLES(value);
                break;
            case 'U':
                   setSHOW_VELOCITY(value);
                break;
            case 'c':
                   setNUMBER_COUNT(value);
                break;
            case 'M':
                   const_MOVING = value;
                break;
            case 'm':
                   const_MAPPING = value;
                break;
            case 's':
                   setSHOW_SENSOR(value);
                break;
            case 'l':
                setRELIABLE_DISTANCE_LEFT(value);
                break;
            case 'r':
                setRELIABLE_DISTANCE_RIGHT(value);
                break;
            case 'n':
                setRELIABLE_DISTANCE_LEFT_IN(value);
                break;
            case 'b':
                setRELIABLE_DISTANCE_RIGHT_IN(value);
                break;
            case 'A':
                setSENSOR_DIFFERENCE_CALIBRATION(value);
                break;
            case 'v':
                setREFERENCE_VELOCITY_FOWARD_FAST(value);
                break;
            case 't':
                setREFERENCE_VELOCITY_TURN_FAST(value);
                break;
            case 'N':
                if (flag_move == 0){
                    direction = NORTH;
                    cells = value;
                    flag_move = 1;
                }

                break;
            case 'W':
                if (flag_move == 0){
                    direction = WEST;
                    cells = value;
                    flag_move = 1;
                }

                break;
            case 'S':
                if (flag_move == 0){
                    direction = SOUTH;
                    cells = value;
                    flag_move = 1;
                }
                break;
            case 'E':
                if (flag_move == 0){
                    direction = EAST;
                    cells = value;
                    flag_move = 1;
                }
                break;
            case 'B':
                const_MOVING = !const_MOVING;
                const_MAPPING = 0;
                flag_button = value;
                break;
            // END OF DELETING
        }
}