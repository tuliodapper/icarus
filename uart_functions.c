#include <p30Fxxxx.h>
#include <stdio.h>

static int error_counter = 0;

int getErrorCounter(void){
    return error_counter;
}

void resetErrorCounter(void){
    error_counter = 0;
}

void incrementErrorCounter(void){
    error_counter++;
}

void decrementErrorCounter(void){
    error_counter--;
}

void rcvHandler (char c, int way) {
    static char rcvArray[40];
    static char rcv0;
    static int i = -2;

    int converted;

    if (c == '<')
    {
        i = -1; //increments vector position index
    } else if (c == '>') {
        rcvArray[i] = '\0'; //adds ending byte to String
        //here comes a Switch to determine which function will be taken according to the start-end byte
        //ex: if it's a *75.34*, then the action is setting this number as the duty cycle, and so on
        sscanf(rcvArray, "%d", &converted);
        uart_receive(rcv0, converted, way);
        rcv0 = '\0'; //erases command id byte
        i = -2; //resets string position index "i"
    } else if (i==-1) {
        rcv0 = c;
        i++;
    } else if (i!=-2){
        rcvArray[i] = c; //adds received char to the array
        i++; //increments vector position index
    }
    
}