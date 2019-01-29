#ifndef CONTROL_H
#define	CONTROL_H

int PID_controller ( int  velocity  );
void setKp(int value);
void setKi(int value);
void setKd(int value);
void setDesiredVelocity(int value);

#endif