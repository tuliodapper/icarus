#include "io.h"

// Declaration and initialisation
unsigned int old_value_S3 = 0;
unsigned int current_value_S3 = 0;

void read_buttons()
{
    static unsigned int i = 0;

    // When the current value (in the program) is diffent from the port value, then the counter is incremented.
    if (get_current_value_s3() != BUTTONPort){
        i++;
        // if the counter is above 10, the current value (in the program) is updated.
        if (i > 10){
            i = 0;
            current_value_S3 = BUTTONPort;
        }
    }
    else
    {
        i=0;
    }

}

void reset_buttons()
{
    old_value_S3 = current_value_S3; // it good to have values in one main as it might get lost
}

unsigned int get_current_value_s3()
{
    return current_value_S3;
}

unsigned int get_old_value_s3()
{
    return old_value_S3;
}


void io_setup()
{
    // Encoders
    ENCATris   = 1;
    ENCBTris   = 1;
    BUTTONTris = 1;

    // #### Outputs ####
    // Leds
    LED1Tris    = 0;
    LED2Tris    = 0;
    // PWMs
    PWMTris    = 0;
    // DIRs
    DIRATris    = 0;
    DIRBTris    = 0;
    // IR Transistors
    FRONT_RIGHT_SENSOR_Tris = 0;
    FRONT_LEFT_SENSOR_Tris = 0;
    DIAGONAL_RIGHT_SENSOR_Tris = 0;
    DIAGONAL_LEFT_SENSOR_Tris = 0;
    RIGHT_SENSOR_Tris = 0;
    LEFT_SENSOR_Tris = 0;

}