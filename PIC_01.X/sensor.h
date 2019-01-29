#ifndef SENSOR_H
#define	SENSOR_H

#define RIGHT_SENSOR 0
#define LEFT_SENSOR 1

#define DIAGONAL_RIGHT_SENSOR 2
#define DIAGONAL_LEFT_SENSOR 3

#define FRONT_RIGHT_SENSOR 4
#define FRONT_LEFT_SENSOR 5

#define NUMBER_SAMPLES 10

#define IDEAL_DISTANCE_FRONT 600
#define IDEAL_DISTANCE_LEFT 421
#define IDEAL_DISTANCE_RIGHT 290

#define RELIABLE_DISTANCE_LEFT 300
#define RELIABLE_DISTANCE_RIGHT 200

#define RELIABLE_DISTANCE_LEFT_IN 300
#define RELIABLE_DISTANCE_RIGHT_IN 200

void sensor_setup();
void Sample(unsigned char S1);
unsigned int getSensorValue(unsigned char S1);
void setSideSensorValue(unsigned char S1, unsigned int value);
unsigned int getSideSensorValue(unsigned char S1);
unsigned char IsThereWallBySensor(unsigned char sensor);
unsigned char IsThereWall(unsigned char direction);
void resetNumberSamples(void);
void WaitSample(unsigned char S1);

// START OF DELETING
void setIDEAL_DISTANCE_FRONT(unsigned int value);
unsigned int getIDEAL_DISTANCE_FRONT(void);
void setIDEAL_DISTANCE_RIGHT(unsigned int value);
unsigned int getIDEAL_DISTANCE_RIGHT(void);
void setIDEAL_DISTANCE_LEFT(unsigned int value);
unsigned int getIDEAL_DISTANCE_LEFT(void);
void setTHRESHOLD_FRONT(unsigned int value);
unsigned int getTHRESHOLD_FRONT(void);
void setTHRESHOLD_DIAGONAL_RIGHT(unsigned int value);
unsigned int getTHRESHOLD_DIAGONAL_RIGHT(void);
void setTHRESHOLD_DIAGONAL_LEFT(unsigned int value);
unsigned int getTHRESHOLD_DIAGONAL_LEFT(void);
void setTHRESHOLD_RIGHT(unsigned int value);
unsigned int getTHRESHOLD_RIGHT(void);
void setTHRESHOLD_LEFT(unsigned int value);
unsigned int getTHRESHOLD_LEFT(void);
void setNUMBER_SAMPLES(unsigned int value);
unsigned int getNUMBER_SAMPLES(void);
void setRELIABLE_DISTANCE_LEFT(unsigned int value);
unsigned int getRELIABLE_DISTANCE_LEFT(void);
void setRELIABLE_DISTANCE_RIGHT(unsigned int value);
unsigned int getRELIABLE_DISTANCE_RIGHT(void);
void setRELIABLE_DISTANCE_LEFT_IN(unsigned int value);
unsigned int getRELIABLE_DISTANCE_LEFT_IN(void);
void setRELIABLE_DISTANCE_RIGHT_IN(unsigned int value);
unsigned int getRELIABLE_DISTANCE_RIGHT_IN(void);
// END OF DELETING

#endif