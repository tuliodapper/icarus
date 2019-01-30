#include "position_control.h"
#include "../delay.h"
#include "io.h"
#include "sensor.h"
#include "timer1.h"
#include "main.h"
#include <stdlib.h>

static unsigned int reference_velocity_foward = 0;
static unsigned int reference_velocity_turn = 0;
static int current_position = 0;
static int desired_position = 0;
static int current_direction = NORTH;
static int current_state = STATE_STOPPED;
static float constant_correction_velocity = 0;
static int flag_stop = 0;
static int flag_pause = 0;
static unsigned int try_1 = 0;
static unsigned int try_2 = 0;


// START OF DELETING
static unsigned int const_DISTANCE_FOWARD = DISTANCE_FOWARD;
static unsigned int const_DISTANCE_FOWARD_MID_CELL_OUT = DISTANCE_FOWARD_MID_CELL_OUT;
static unsigned int const_DISTANCE_FOWARD_MID_CELL_IN = DISTANCE_FOWARD_MID_CELL_IN;
static unsigned int const_DISTANCE_FOWARD_STOP_WALL = DISTANCE_FOWARD_STOP_WALL;
static unsigned int const_DISTANCE_TURN = DISTANCE_TURN;
static unsigned int const_REFERENCE_VELOCITY_FOWARD = REFERENCE_VELOCITY_FOWARD;
static unsigned int const_REFERENCE_VELOCITY_TURN = REFERENCE_VELOCITY_TURN;
static unsigned int const_REFERENCE_VELOCITY_FOWARD_FAST = REFERENCE_VELOCITY_FOWARD_FAST;
static unsigned int const_REFERENCE_VELOCITY_TURN_FAST = REFERENCE_VELOCITY_TURN_FAST;
static float const_CONSTANT_CORRECTION_VELOCITY = CONSTANT_CORRECTION_VELOCITY;
static unsigned int const_CONSTANT_CORRECTION_VELOCITY_DIVISOR = 1000;

void setDISTANCE_FOWARD(unsigned int value){
    const_DISTANCE_FOWARD = value;
}
void setDISTANCE_FOWARD_MID_CELL_OUT(unsigned int value){
    const_DISTANCE_FOWARD_MID_CELL_OUT = value;
}
void setDISTANCE_FOWARD_MID_CELL_IN(unsigned int value){
    const_DISTANCE_FOWARD_MID_CELL_IN= value;
}
void setDISTANCE_FOWARD_STOP_WALL(unsigned int value){
    const_DISTANCE_FOWARD_STOP_WALL = value;
}
void setDISTANCE_TURN(unsigned int value){
    const_DISTANCE_TURN = value;
}
void setREFERENCE_VELOCITY_FOWARD(unsigned int value){
    const_REFERENCE_VELOCITY_FOWARD = value;
}
void setREFERENCE_VELOCITY_TURN(unsigned int value){
    const_REFERENCE_VELOCITY_TURN = value;
}
void setREFERENCE_VELOCITY_FOWARD_FAST(unsigned int value){
    const_REFERENCE_VELOCITY_FOWARD_FAST = value;
}
void setREFERENCE_VELOCITY_TURN_FAST(unsigned int value){
    const_REFERENCE_VELOCITY_TURN_FAST = value;
}
void setCONSTANT_CORRECTION_VELOCITY(unsigned int value){
    const_CONSTANT_CORRECTION_VELOCITY = (float) value / const_CONSTANT_CORRECTION_VELOCITY_DIVISOR;
}
void setCONSTANT_CORRECTION_VELOCITY_DIVISOR(unsigned int value){
    const_CONSTANT_CORRECTION_VELOCITY_DIVISOR = value;
}
// END OF DELETING
int getCurrentDirection(void){
    return current_direction;
}

void addPosition(int value){
    current_position = current_position + value;
    
    if (abs(current_position) >= abs(desired_position))
    {
        flag_stop = 1;
        try_1 = getSensorValue(FRONT_LEFT_SENSOR);
        try_2 = getSensorValue(FRONT_RIGHT_SENSOR);
    }
    else
        if (current_state == STATE_MOVING_FORWARD) {
            if (((getSensorValue(FRONT_LEFT_SENSOR) + getSensorValue(FRONT_RIGHT_SENSOR))/2) > getIDEAL_DISTANCE_FRONT())
            {
                // if (abs(current_position) >= (abs(desired_position) - const_DISTANCE_FOWARD_STOP_WALL))
                    flag_stop = 1;
            }
            
                //  else
                //      (flag_pause = 1);
                //}
                //    else (flag_pause = 0);
        }
}

unsigned int Rounding(float value)
{
    unsigned int value_int = value;
    if( (value - value_int) >= 0.5) return (value_int +1);
    else return value_int;
}

void UpdatePosition(char trigger){
    // current_position = Rounding(current_position / const_DISTANCE_FOWARD) * const_DISTANCE_FOWARD;
    if (trigger == TRIGGER_OUT)
        current_position = const_DISTANCE_FOWARD_MID_CELL_OUT;
    else
        current_position = const_DISTANCE_FOWARD_MID_CELL_IN;
}

unsigned int getReferenceVelocityFoward(void){
    return reference_velocity_foward;
}

unsigned int getReferenceVelocityTurn(void){
    return reference_velocity_turn;
}

float getConstCorrectionVelocity(void){
    return constant_correction_velocity;
}

unsigned int getFlagStop(void){
    return flag_stop;
}

void ChangeDirection(unsigned int direction)
{
    current_direction = direction;
}

void ResetParameters(void)
{
    current_state = STATE_STOPPED;
    reference_velocity_foward = 0;
    reference_velocity_turn = 0;
    current_position = 0;
    desired_position = 0;
    flag_stop = 0;
    constant_correction_velocity = 0;
}

void MoveFowardBySpeed(unsigned int cells, unsigned int velocity){
    int i;
    ResetParameters();
    current_state = STATE_MOVING_FORWARD;
    desired_position = const_DISTANCE_FOWARD;
    reference_velocity_foward = velocity;
    constant_correction_velocity = const_CONSTANT_CORRECTION_VELOCITY;
    for (i=0;i<cells;i++){
        while(flag_stop == 0);
        current_position = 0;
        flag_stop = 0;
    }
    ResetParameters();
}

void MoveFoward(unsigned int cells, unsigned char fast)
{
    if (fast) MoveFowardBySpeed(cells, const_REFERENCE_VELOCITY_FOWARD_FAST);
    else MoveFowardBySpeed(cells, const_REFERENCE_VELOCITY_FOWARD);

    if (IsThereWallBySensor(FRONT_LEFT_SENSOR) && (((try_1 + try_2)/2) < getIDEAL_DISTANCE_FRONT()))
        if (fast) MoveFowardBySpeed(1, const_REFERENCE_VELOCITY_FOWARD_FAST);
        else MoveFowardBySpeed(1, const_REFERENCE_VELOCITY_FOWARD);
}

unsigned int InvertDirection(unsigned int direction)
{
    if (direction == NORTH)
        return (SOUTH);
    else if (direction == SOUTH)
        return (NORTH);
    else if (direction == WEST)
        return (EAST);
    else
        return (WEST);
}

unsigned int NextDirection(unsigned int current_direction, unsigned int right_left){
    unsigned int response;
    switch(current_direction){
            case NORTH: if (right_left == RIGHT) response = EAST; else response = WEST;
                        break;
            case WEST: if (right_left == RIGHT) response = NORTH; else response = SOUTH;
                        break;
            case SOUTH: if (right_left == RIGHT) response = WEST; else response = EAST;
                        break;
            case EAST: if (right_left == RIGHT) response = SOUTH; else response = NORTH;
                        break;
    }
    return response;
}

unsigned int TurnRightOrLeft(unsigned int current_direction, unsigned int desired_direction)
{
    unsigned int response = RIGHT;
    if (((current_direction == NORTH) && (desired_direction == WEST)) ||
            ((current_direction == WEST) && (desired_direction == SOUTH)) ||
                ((current_direction == SOUTH) && (desired_direction == EAST)) ||
                    ((current_direction == EAST) && (desired_direction == NORTH)))
            response = LEFT;
    return response;
}


void TurnDirection(unsigned int desired_direction)
{
    unsigned int RightLeft;
    unsigned char flag_turned = FALSE;

    ResetParameters();
    while(current_direction!=desired_direction){
        flag_turned = TRUE;
        current_state = STATE_MOVING_TURN;
        RightLeft = TurnRightOrLeft(current_direction, desired_direction);
        if(RightLeft == RIGHT)
        {
             desired_position = const_DISTANCE_TURN;
             reference_velocity_turn = const_REFERENCE_VELOCITY_TURN;
             
        } else
        {
             desired_position = (-1)*const_DISTANCE_TURN;
             reference_velocity_turn = (-1)*const_REFERENCE_VELOCITY_TURN;
        }
        while(flag_stop == 0);
        ResetParameters();
        current_direction = NextDirection(current_direction, RightLeft);
    }
    
    if (flag_turned){
//        resetNumberSamples();
//        WaitSample(LEFT_SENSOR);
//        WaitSample(RIGHT_SENSOR);

    LEFT_SENSOR_Latch = 1;
    Sample(LEFT_SENSOR);
    LEFT_SENSOR_Latch = 0;

    RIGHT_SENSOR_Latch = 1;
    Sample(RIGHT_SENSOR);
    RIGHT_SENSOR_Latch = 0;

        updateSideWall();
        updateWallState();
    }
}

void Move(unsigned int desired_direction, unsigned int cells, unsigned char fast){
    TurnDirection(desired_direction);
    MoveFoward(cells, fast);
}