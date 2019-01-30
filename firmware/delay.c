#include "delay.h"

void myDelay(unsigned int inTimeMiliSeconds)
{
    unsigned int i, j;
    for (i=0;i<inTimeMiliSeconds;i++)
        for (j=0;j<1998;j++);
}
