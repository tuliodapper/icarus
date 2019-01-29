#include <p30Fxxxx.h> //picks the correct device automatically
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

void setLED(unsigned int LED){
    if (LED == 3)
        LED3Latch = 1;
    else if (LED == 4)
        LED4Latch = 1;
}
void resetLED(unsigned int LED){
    if (LED == 3)
        LED3Latch = 0;
    else if (LED == 4)
        LED4Latch = 0;
}
void toggleLED(unsigned int LED){
    if (LED == 3)
        LED3Latch = !LED3Latch;
    else if (LED == 4)
        LED4Latch = !LED3Latch;
}

void io_setup()
{
    // Configuration of inputs
    BUTTONTris  = 1;
    // Encoders
    ENCATris    = 1;
    ENCBTris    = 1;

    // #### Outputs ####
    // Leds
    LED3Tris    = 0;
    LED4Tris    = 0;
    // PWMs
    PWMTris    = 0;
    // DIRs
    DIRATris    = 0;
    DIRBTris    = 0;

}