/*
 * File:   Timer1Functions.h
 * Author: tulio
 *
 * Created on 29 October 2014, 01:19
 */

#ifndef TIMER1_H
#define	TIMER1_H

#define WALL_STATE_DOUBLE_WALL 0
#define WALL_STATE_LEFT_WALL 1
#define WALL_STATE_RIGHT_WALL 2
#define WALL_STATE_NO_WALL 3

#define TRUE 1
#define FALSE 0

#define TRIGGER_OUT 0
#define TRIGGER_IN 1

void timer1_setup();
void setSHOW_VELOCITY(unsigned int value);
void setSHOW_SENSOR(unsigned int value);
void setSENSOR_DIFFERENCE_CALIBRATION(int value);
unsigned int getSHOW_SENSOR();
void setNUMBER_COUNT(unsigned int value);
void setDeadZone(unsigned int value);
void updateSideWall(void);
void updateWallState(void);

#endif	/* TIMER1_H */