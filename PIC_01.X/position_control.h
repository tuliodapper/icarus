/* 
 * File:   position_control.h
 * Author: TúlioDapper
 *
 * Created on 28 January 2015, 22:14
 */

#ifndef POSITION_CONTROL_H
#define	POSITION_CONTROL_H

#define DISTANCE_FOWARD 2800
#define DISTANCE_FOWARD_MID_CELL_OUT 1400
#define DISTANCE_FOWARD_MID_CELL_IN 1800
#define DISTANCE_FOWARD_STOP_WALL 500
#define DISTANCE_TURN 1050
#define REFERENCE_VELOCITY_FOWARD 15
#define REFERENCE_VELOCITY_TURN 10
#define REFERENCE_VELOCITY_FOWARD_FAST 15
#define REFERENCE_VELOCITY_TURN_FAST 10
#define CONSTANT_CORRECTION_VELOCITY 0.06 // = (10/512)

#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3

#define LEFT 0
#define RIGHT 1

#define STATE_STOPPED 0 // = (10/512)
#define STATE_MOVING_FORWARD 1 // = (10/512)
#define STATE_MOVING_TURN 2 // = (10/512)

void addPosition(int value);
void UpdatePosition(char trigger);
unsigned int getReferenceVelocityFoward(void);
unsigned int getReferenceVelocityTurn(void);
float getConstCorrectionVelocity(void);
unsigned int getFlagStop(void);
void Move(unsigned int desired_direction, unsigned int cells, unsigned char fast);
int getCurrentDirection(void);
unsigned int TurnRightOrLeft(unsigned int current_direction, unsigned int desired_direction);
unsigned int InvertDirection(unsigned int direction);
unsigned int NextDirection(unsigned int current_direction, unsigned int right_left);
void ChangeDirection(unsigned int direction);
void ResetParameters(void);

// START OF DELETING
void setDISTANCE_FOWARD(unsigned int value);
void setDISTANCE_FOWARD_MID_CELL_IN(unsigned int value);
void setDISTANCE_FOWARD_MID_CELL_OUT(unsigned int value);
void setDISTANCE_FOWARD_STOP_WALL(unsigned int value);
void setDISTANCE_TURN(unsigned int value);
void setREFERENCE_VELOCITY_FOWARD(unsigned int value);
void setREFERENCE_VELOCITY_TURN(unsigned int value);
void setCONSTANT_CORRECTION_VELOCITY(unsigned int value);
void setCONSTANT_CORRECTION_VELOCITY_DIVISOR(unsigned int value);
void setREFERENCE_VELOCITY_FOWARD_FAST(unsigned int value);
void setREFERENCE_VELOCITY_TURN_FAST(unsigned int value);
// END OF DELETING

#endif	/* POSITION_CONTROL_H */

