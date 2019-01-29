#include <p30Fxxxx.h>
#include "sensor.h"
#include "position_control.h"
#include "adc.h"
#include "io.h"

struct sensor {
    unsigned int channel;
    unsigned int threshold;
    unsigned int value;
};

struct side_sensor {
    unsigned char number_samples;
    unsigned int values[NUMBER_SAMPLES];
};

struct sensor array_sensor[6];

struct side_sensor array_side_sensor[2];

// START OF DELETING
static unsigned int const_IDEAL_DISTANCE_FRONT = IDEAL_DISTANCE_FRONT;
static unsigned int const_IDEAL_DISTANCE_LEFT = IDEAL_DISTANCE_LEFT;
static unsigned int const_IDEAL_DISTANCE_RIGHT = IDEAL_DISTANCE_RIGHT;
static unsigned int const_RELIABLE_DISTANCE_LEFT = RELIABLE_DISTANCE_LEFT;
static unsigned int const_RELIABLE_DISTANCE_RIGHT = RELIABLE_DISTANCE_RIGHT;
static unsigned int const_RELIABLE_DISTANCE_LEFT_IN = RELIABLE_DISTANCE_LEFT_IN;
static unsigned int const_RELIABLE_DISTANCE_RIGHT_IN = RELIABLE_DISTANCE_RIGHT_IN;
static unsigned int const_NUMBER_SAMPLES = NUMBER_SAMPLES;

void setRELIABLE_DISTANCE_LEFT(unsigned int value){
    const_RELIABLE_DISTANCE_LEFT = value;
}
unsigned int getRELIABLE_DISTANCE_LEFT(void){
    return const_RELIABLE_DISTANCE_LEFT;
}
void setRELIABLE_DISTANCE_RIGHT(unsigned int value){
    const_RELIABLE_DISTANCE_RIGHT = value;
}
unsigned int getRELIABLE_DISTANCE_RIGHT(void){
    return const_RELIABLE_DISTANCE_RIGHT;
}
void setRELIABLE_DISTANCE_LEFT_IN(unsigned int value){
    const_RELIABLE_DISTANCE_LEFT_IN = value;
}
unsigned int getRELIABLE_DISTANCE_LEFT_IN(void){
    return const_RELIABLE_DISTANCE_LEFT_IN;
}
void setRELIABLE_DISTANCE_RIGHT_IN(unsigned int value){
    const_RELIABLE_DISTANCE_RIGHT_IN = value;
}
unsigned int getRELIABLE_DISTANCE_RIGHT_IN(void){
    return const_RELIABLE_DISTANCE_RIGHT_IN;
}
void setIDEAL_DISTANCE_FRONT(unsigned int value){
    const_IDEAL_DISTANCE_FRONT = value;
}
unsigned int getIDEAL_DISTANCE_FRONT(void){
    return const_IDEAL_DISTANCE_FRONT;
}
void setIDEAL_DISTANCE_LEFT(unsigned int value){
    const_IDEAL_DISTANCE_LEFT = value;
}
unsigned int getIDEAL_DISTANCE_LEFT(void){
    return const_IDEAL_DISTANCE_LEFT;
}
void setIDEAL_DISTANCE_RIGHT(unsigned int value){
    const_IDEAL_DISTANCE_RIGHT = value;
}
unsigned int getIDEAL_DISTANCE_RIGHT(void){
    return const_IDEAL_DISTANCE_RIGHT;
}
void setTHRESHOLD_FRONT(unsigned int value){
    array_sensor[FRONT_RIGHT_SENSOR].threshold = value;
    array_sensor[FRONT_LEFT_SENSOR].threshold = value;
}
unsigned int getTHRESHOLD_FRONT(void){
    return array_sensor[FRONT_RIGHT_SENSOR].threshold;
}
void setTHRESHOLD_DIAGONAL_RIGHT(unsigned int value){
    array_sensor[DIAGONAL_RIGHT_SENSOR].threshold = value;
}
unsigned int getTHRESHOLD_DIAGONAL_RIGHT(void){
    return array_sensor[DIAGONAL_RIGHT_SENSOR].threshold;
}
void setTHRESHOLD_DIAGONAL_LEFT(unsigned int value){
    array_sensor[DIAGONAL_LEFT_SENSOR].threshold = value;
}
unsigned int getTHRESHOLD_DIAGONAL_LEFT(void){
    return array_sensor[DIAGONAL_LEFT_SENSOR].threshold;
}
void setTHRESHOLD_RIGHT(unsigned int value){
    array_sensor[RIGHT_SENSOR].threshold = value;
}
unsigned int getTHRESHOLD_RIGHT(void){
    return array_sensor[RIGHT_SENSOR].threshold;
}
void setTHRESHOLD_LEFT(unsigned int value){
    array_sensor[LEFT_SENSOR].threshold = value;
}
unsigned int getTHRESHOLD_LEFT(void){
    return array_sensor[LEFT_SENSOR].threshold;
}
void setNUMBER_SAMPLES(unsigned int value){
    const_NUMBER_SAMPLES = value;
}
unsigned int getNUMBER_SAMPLES(void){
    return const_NUMBER_SAMPLES;
}
// END OF DELETING

void sensor_setup(){
    array_sensor[FRONT_RIGHT_SENSOR].channel = 0b0000000000000010;
    array_sensor[FRONT_RIGHT_SENSOR].value = 0;
    array_sensor[FRONT_RIGHT_SENSOR].threshold = 200;

    array_sensor[FRONT_LEFT_SENSOR].channel = 0b0000000100000000;
    array_sensor[FRONT_LEFT_SENSOR].value = 0;
    array_sensor[FRONT_LEFT_SENSOR].threshold = 200;

    array_sensor[DIAGONAL_RIGHT_SENSOR].channel = 0b0000000000000100;
    array_sensor[DIAGONAL_RIGHT_SENSOR].value = 0;
    array_sensor[DIAGONAL_RIGHT_SENSOR].threshold = 100;

    array_sensor[DIAGONAL_LEFT_SENSOR].channel = 0b0000000010000000;
    array_sensor[DIAGONAL_LEFT_SENSOR].value = 0;
    array_sensor[DIAGONAL_LEFT_SENSOR].threshold = 50;

    array_sensor[RIGHT_SENSOR].channel = 0b0000000000001000;
    array_sensor[RIGHT_SENSOR].value = 0;
    array_sensor[RIGHT_SENSOR].threshold = 150;//100
    array_side_sensor[RIGHT_SENSOR].number_samples = 0;

    array_sensor[LEFT_SENSOR].channel = 0b0000000001000000;
    array_sensor[LEFT_SENSOR].value = 0;
    array_sensor[LEFT_SENSOR].threshold = 200; //100
    array_side_sensor[LEFT_SENSOR].number_samples = 0;
}

void Sample(unsigned char S1){
    ADCON1bits.ADON=0;
    ADCSSL=(array_sensor[S1].channel);
    ADCON1bits.ADON=1;
    resetFlagInterrupt();
    ADCON1bits.ASAM=1; //stop the sampling
    while(!getFlagInterrupt());
    ADCON1bits.ASAM=0; //stop the sampling
    array_sensor[S1].value = ADCBUF0;
}

void setSideSensorValue(unsigned char S1, unsigned int value){
    unsigned char i;
    if (array_side_sensor[S1].number_samples < const_NUMBER_SAMPLES)
        array_side_sensor[S1].number_samples ++;
    for (i=(array_side_sensor[S1].number_samples-1);i>0;i--)
        array_side_sensor[S1].values[i]=array_side_sensor[S1].values[i-1];
    array_side_sensor[S1].values[0] = value;
}

void resetNumberSamples(void){
    array_side_sensor[LEFT_SENSOR].number_samples = 0;
    array_side_sensor[RIGHT_SENSOR].number_samples = 0;
}

unsigned int getSideSensorValue(unsigned char S1){
    unsigned char i;
    unsigned int value = 0;
    for (i=0;i<array_side_sensor[S1].number_samples;i++)
        value=value+array_side_sensor[S1].values[i];
    value=(unsigned int) value/array_side_sensor[S1].number_samples;
    return value;
}

void WaitSample(unsigned char S1){
    while(array_side_sensor[S1].number_samples == 0);
}

unsigned int getSensorValue(unsigned char S1){
    return array_sensor[S1].value;
}

unsigned char IsThereWallBySensor(unsigned char sensor){
    if ((sensor == FRONT_LEFT_SENSOR)||(sensor == FRONT_RIGHT_SENSOR))
        return (((getSensorValue(FRONT_LEFT_SENSOR) + getSensorValue(FRONT_RIGHT_SENSOR))/2) > array_sensor[FRONT_RIGHT_SENSOR].threshold);
    else
        return (getSensorValue(sensor) > array_sensor[sensor].threshold);
}

unsigned char IsThereWall(unsigned char direction){
    if (getCurrentDirection() == direction) return IsThereWallBySensor(FRONT_LEFT_SENSOR);
    else if (TurnRightOrLeft(getCurrentDirection(), direction) == RIGHT) return IsThereWallBySensor(RIGHT_SENSOR);
    else return IsThereWallBySensor(LEFT_SENSOR);
 }