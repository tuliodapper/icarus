// PIC 01

#include <p30Fxxxx.h>
#include "main.h"
#include "adc.h"
#include "io.h"
#include "position_control.h"
#include "uart.h"
#include "sensor.h"
#include "timer1.h"
#include "bluetooth_setup.h"
#include "../pwm_setup.h"
#include "../timer1_setup.h"
#include "../uart1_setup.h"
#include "../uart2_setup.h"
#include "../uart_functions.h"
#include "../qei.h"
#include "../delay.h"
#include "../velocity_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_FOSC(PRI & XT_PLL8 & CSW_FSCM_OFF);

_FWDT(WDT_OFF);

_FBORPOR(PBOR_OFF & MCLR_EN & PWRT_64 & PWMxL_ACT_HI & PWMxH_ACT_LO & RST_PWMPIN);

struct Cell {
    unsigned char distance; //indicates the distance to destination point
    unsigned char walls; //POSITION west=0 1=north 2=east 3=south
    unsigned char visited; //variable indicates has been in this cell or not
};

static struct Cell Maze[MAZE_SIZE_X][MAZE_SIZE_Y];

static unsigned char mode_operation;


void sendParameterInfo(int x, int y, int walls) {
    char str[15];
    sprintf(str, "%02d%02d%02d\r\n", x, y, walls);
    uart1_mySendString(str);
}

void sendINFO2PC(unsigned char current_x, unsigned char current_y, unsigned char walls) {
//    incrementErrorCounter();
//
//    while (getErrorCounter() > 0);
//
//    resetErrorCounter();

    sendParameterInfo(current_x, current_y, walls);

}

void defineWall(unsigned char x, unsigned char y, unsigned char direction, unsigned char wall) {
    if ((x >= 0) && (x < MAZE_SIZE_X) && (y >= 0) && (y < MAZE_SIZE_Y)) {
        if (wall) Maze[x][y].walls |= (1 << direction);
        else Maze[x][y].walls &= ~(1 << direction);
    }
}

void defineDuoWall(unsigned char x, unsigned char y, unsigned char direction, unsigned char wall) {

    unsigned char new_x, new_y;
    defineWall(x, y, direction, wall);
    newxy_from_xy(direction, 1, x, y, &new_x, &new_y);
    defineWall(new_x, new_y, invert_direction(direction), wall);
}

unsigned char getWall(unsigned char x, unsigned char y, unsigned char direction) {
    return (Maze[x][y].walls & (1 << direction));
}

void newxy_from_xy(unsigned char direction, unsigned char no_cells, unsigned char current_x, unsigned char current_y, unsigned char * new_x, unsigned char * new_y) {
    if (direction == NORTH) {
        *new_x = current_x;
        *new_y = current_y + no_cells;
    } else if (direction == EAST) {
        *new_x = current_x + no_cells;
        *new_y = current_y;
    } else if (direction == SOUTH) {
        *new_x = current_x;
        *new_y = current_y - no_cells;
    } else {
        *new_x = current_x - no_cells;
        *new_y = current_y;
    }
}

void update_distance_value(void) {

    unsigned char x;
    unsigned char y;

    unsigned char direction;
    unsigned char new_x;
    unsigned char new_y;
    unsigned char level;

    unsigned char ff_stack[MAZE_SIZE_Y * MAZE_SIZE_X][2];
    int ff_counter = -1;
    unsigned char wall_stack[MAZE_SIZE_Y * MAZE_SIZE_X][2];
    int wall_counter = -1;

    for (x = 0; x < MAZE_SIZE_X; x++)
        for (y = 0; y < MAZE_SIZE_Y; y++)
            Maze[x][y].distance = 255;

    level = 0;

    if ((mode_operation == MO1_MAPPING_GOING_TO_CENTRE) || (mode_operation == MO3_QUICKEST_PATH_GOING_TO_CENTRE)) {
        for (x = (MAZE_SIZE_X / 2) - 1; x < ((MAZE_SIZE_X / 2) + 1); x++)
            for (y = (MAZE_SIZE_Y / 2) - 1; y < ((MAZE_SIZE_Y / 2) + 1); y++) {
                ff_counter++;
                ff_stack[ff_counter][0] = x;
                ff_stack[ff_counter][1] = y;
            }// setting zeros to centre.
    } else {
        ff_counter = 0;
        ff_stack[ff_counter][0] = START_POSITION_X;
        ff_stack[ff_counter][1] = START_POSITION_Y;
    }

    //repeat untill ff stack is empty
    while (ff_counter > -1) {
        while (ff_counter > -1) {
            x = ff_stack[ff_counter][0];
            y = ff_stack[ff_counter][1];
            ff_counter--;
            if (Maze[x][y].distance == 255) {
                Maze[x][y].distance = level;
                //OPEN_NEIGHBOUR_FUNCTION
                for (direction = 0; direction < 4; direction++) {
                    if (getWall(x, y, direction) == FALSE) {
                        wall_counter++;
                        //new_x and new_y
                        newxy_from_xy(direction, 1, x, y, &new_x, &new_y);
                        wall_stack[wall_counter][0] = new_x;
                        wall_stack[wall_counter][1] = new_y;
                    }
                }
            }
        }

        // The array ff_stack is now empty

        if (wall_counter > -1) {
            level++;
            memcpy(ff_stack, wall_stack, sizeof (ff_stack));
            ff_counter = wall_counter;
            wall_counter = -1;
        }
    }
}

unsigned char direction_from_xy(unsigned char current_x, unsigned char current_y, unsigned char new_x, unsigned char new_y) {
    if ((new_x > current_x) && (new_y == current_y))
        return EAST;
    else if ((new_x < current_x) && (new_y == current_y))
        return WEST;
    else if ((new_x == current_x) && (new_y > current_y))
        return NORTH;
    else
        return SOUTH;

}

void find_open_neighbour_cell(unsigned char current_x, unsigned char current_y, unsigned char ignore_not_visited_cells, unsigned char * direction) {

    unsigned char stack_open_neighbour[4][2];
    int stack_open_neighbour_position = -1;

    unsigned char new_x, new_y;
    unsigned char i;

    for (i = 0; i < 4; i++) {
        if (getWall(current_x, current_y, i) == FALSE) {
            newxy_from_xy(i, 1, current_x, current_y, &new_x, &new_y);
            if ((Maze[new_x][new_y].visited == TRUE) || (ignore_not_visited_cells == FALSE)) {
                stack_open_neighbour_position++;
                //new_x and new_y
                stack_open_neighbour[stack_open_neighbour_position][0] = new_x;
                stack_open_neighbour[stack_open_neighbour_position][1] = new_y;
            }
        }
    }


    unsigned char position_lowest = 0;
    for (i = 1; i < (stack_open_neighbour_position + 1); i++)
        if (Maze[stack_open_neighbour[i][0]][stack_open_neighbour[i][1]].distance < Maze[stack_open_neighbour[position_lowest][0]][stack_open_neighbour[position_lowest][1]].distance)
            position_lowest = i;

    *direction = direction_from_xy(current_x, current_y, stack_open_neighbour[position_lowest][0], stack_open_neighbour[position_lowest][1]);
}

void find_the_next_movement(unsigned char current_x, unsigned char current_y, unsigned char * no_cells, unsigned char * direction) {
    unsigned char next_direction;
    *no_cells = 0;

    find_open_neighbour_cell(current_x, current_y, TRUE, direction);
    newxy_from_xy(*direction, 1, current_x, current_y, &current_x, &current_y);

    next_direction = *direction;

    while (*direction == next_direction) {
        (*no_cells)++;
        find_open_neighbour_cell(current_x, current_y, TRUE, &next_direction);
        newxy_from_xy(next_direction, 1, current_x, current_y, &current_x, &current_y);
    }

}

unsigned char invert_direction(unsigned char direction) {
    if (direction == NORTH)
        return (SOUTH);
    else if (direction == SOUTH)
        return (NORTH);
    else if (direction == WEST)
        return (EAST);
    else
        return (WEST);
}

void store_walls(unsigned char current_x, unsigned char current_y) {
    FRONT_LEFT_SENSOR_Latch = 1;
    Sample(FRONT_LEFT_SENSOR);
    FRONT_LEFT_SENSOR_Latch = 0;

    FRONT_RIGHT_SENSOR_Latch = 1;
    Sample(FRONT_RIGHT_SENSOR);
    FRONT_RIGHT_SENSOR_Latch = 0;

    LEFT_SENSOR_Latch = 1;
    Sample(LEFT_SENSOR);
    LEFT_SENSOR_Latch = 0;

    RIGHT_SENSOR_Latch = 1;
    Sample(RIGHT_SENSOR);
    RIGHT_SENSOR_Latch = 0;

    defineDuoWall(current_x, current_y, getCurrentDirection(), IsThereWallBySensor(FRONT_LEFT_SENSOR));
    defineDuoWall(current_x, current_y, NextDirection(getCurrentDirection(), LEFT), IsThereWallBySensor(LEFT_SENSOR));
    defineDuoWall(current_x, current_y, NextDirection(getCurrentDirection(), RIGHT), IsThereWallBySensor(RIGHT_SENSOR));
}

void sendParameterToPIC02(char c, int value) {
    uart2_sendParameter(c, value);
    incrementErrorCounter();
    myDelay(500);
    while (getErrorCounter() > 0) {
        resetErrorCounter();
        uart2_sendParameter(c, value);
        incrementErrorCounter();
        myDelay(500);
    }
}

void ResetLocation(unsigned char * current_x, unsigned char * current_y) {

    *current_x = START_POSITION_X;

    *current_y = START_POSITION_Y;

    ChangeDirection(NORTH);
    
}

void Reset(unsigned char * current_x, unsigned char * current_y, unsigned char * update_bit) {

    unsigned char x; //center is the mod op 1st and 3rd goal
    unsigned char y;
    unsigned char i;

    ResetLocation(current_x, current_y);

    *update_bit = 0;

    mode_operation = MO1_MAPPING_GOING_TO_CENTRE;

    for (x = 0; x < MAZE_SIZE_X; x++)
        for (y = 0; y < MAZE_SIZE_Y; y++) {
            Maze[x][y].walls = 0; //indicates the distance to destination point
            Maze[x][y].distance = 0; //indicates the distance to destination point
            Maze[x][y].visited = 0; //variable indicates has been in this cell or not
        }

    /* ##### setting outer walls ##### */
    for (i = 0; i < MAZE_SIZE_X; i++) {
        defineWall(i, 0, SOUTH, TRUE); // wall
        defineWall(i, MAZE_SIZE_Y - 1, NORTH, TRUE); //east wall
    }
    for (i = 0; i < MAZE_SIZE_Y; i++) {
        defineWall(0, i, WEST, TRUE); //west wall
        defineWall(MAZE_SIZE_X - 1, i, EAST, TRUE); //south wall
    }
    /* ##### end of setting wall ##### */

   ResetParameters();

   if (getUART_MAPPING()) sendINFO2PC(99, 99, 0);

}

int main(void) {

    unsigned char update_bit = 0;

    unsigned char current_x = START_POSITION_X;
    unsigned char current_y = START_POSITION_Y;

    unsigned char centerx; //center is the mod op 1st and 3rd goal
    unsigned char centery;

    unsigned char no_cells;

    unsigned char direction;

    unsigned char i;

    io_setup();

    if (CONFIGURE_BLUETOOTH) {
        uart1_setup(BAUDRATE_BLUETOOTH_CONFIGURATION_MODE);
        bluetooth_setup();
    }

    uart1_setup(BAUDRATE_BLUETOOTH);
    uart2_setup(BAUDRATE_PIC);
    
    setKp(CONTROL_KP);
    sendParameterToPIC02('P', CONTROL_KP);
    setKi(CONTROL_KI);
    sendParameterToPIC02('I', CONTROL_KI);
    setKd(CONTROL_KD);
    sendParameterToPIC02('D', CONTROL_KD);

    pwm_setup();

    qei_setup();

    timer1_setup();

    adc_setup();

    sensor_setup();

    Reset(&current_x, &current_y, &update_bit);

        while (1) {

            if (getUART_MOVING() == 2) { // STOP
                Reset(&current_x, &current_y, &update_bit);
                setUART_MOVING(0);
            }

            if (getUART_MOVING() == 1) { // MOVE

                /* ##### is this Final mode operation? ##### */
                if (mode_operation != MO4_FINISHED) {

                    /* ##### is this mode operation 3? ##### */
                    if (mode_operation == MO3_QUICKEST_PATH_GOING_TO_CENTRE) {
                        //check update bit set
                        if (update_bit == 0) {
                            //update the distance value
                            update_distance_value();
                            //set update
                            update_bit = 1;
                        }

                        //ignore cell not visited
                        //find direction and number of cells for next movement
                        find_the_next_movement(current_x, current_y, &no_cells, &direction);

                    } else {
                        Maze[current_x][current_y].visited = 1;
                        //check for wall
                        store_walls(current_x, current_y);
                        //updating distance value
                        update_distance_value();
                        //Find the open neighbour cell with the lowest distance value
                        no_cells = 1;
                        find_open_neighbour_cell(current_x, current_y, FALSE, &direction);

                    }

                    if (getUART_MAPPING()) sendINFO2PC(current_x, current_y, Maze[current_x][current_y].walls);

                    Move(direction, no_cells, (mode_operation == MO3_QUICKEST_PATH_GOING_TO_CENTRE));

                    newxy_from_xy(direction, no_cells, current_x, current_y, &current_x, &current_y);

                    //is it goal position, is it mode operation1 yes? declare walls on goal
                    if (Maze[current_x][current_y].distance == 0) {

                        if (mode_operation == MO1_MAPPING_GOING_TO_CENTRE) {
                            for (centerx = (MAZE_SIZE_X / 2) - 1; centerx < ((MAZE_SIZE_X / 2) + 1); centerx++)
                                for (centery = (MAZE_SIZE_Y / 2) - 1; centery < ((MAZE_SIZE_Y / 2) + 1); centery++)
                                    for (i = 0; i < 4; i++)
                                        defineDuoWall(centerx, centery, i, TRUE);
                            defineDuoWall(current_x, current_y, invert_direction(direction), FALSE);
                        }
                        mode_operation++;
                    }
                } else {
                    if (getUART_MAPPING()) sendINFO2PC(current_x, current_y, Maze[current_x][current_y].walls);
                    // Celebrating
                    LED1Latch = 0;
                    LED2Latch = 0;
                    for (i = 0; i < 3; i++){
                        LED1Latch = 1;
                        myDelay(CELEBRATION_TIME_INTERVAL);
                        LED1Latch = 0;
                        myDelay(CELEBRATION_TIME_INTERVAL);
                        LED2Latch = 1;
                        myDelay(CELEBRATION_TIME_INTERVAL);
                        LED2Latch = 0;
                        myDelay(CELEBRATION_TIME_INTERVAL);
                    }
                    for (i = 0; i < 3; i++){
                        LED1Latch = 0;
                        LED2Latch = 0;
                        myDelay(CELEBRATION_TIME_INTERVAL);
                        LED1Latch = 1;
                        LED2Latch = 1;
                        myDelay(CELEBRATION_TIME_INTERVAL);
                    }
                    ResetLocation(&current_x, &current_y);
                    mode_operation = MO3_QUICKEST_PATH_GOING_TO_CENTRE;
                    setUART_MOVING(0);
                    if (getUART_MAPPING()) sendINFO2PC(98, 98, 0);
                }
            }

            if (getUART_MOVING() == 0) { // STOP

                // Delay aiming to have 1ms between samples.
                myDelay(1);

                // Verify how many samples the button is (un)pressed and the current value can be updated.
                read_buttons();

                // If the current value and the old value are in this configuration, so, act.
                if ((get_old_value_s3() == 1) && (get_current_value_s3() == 0))
                {
                    // Invert the output.
                    setUART_MOVING(2);
                }

                // Makes the old value equal to the current value
                reset_buttons();
            }

        }


    return 0;
}