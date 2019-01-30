#include "../uart1_setup.h"
#include "../delay.h"
#include <stdio.h>

void sendMessage(char* message){
    uart1_mySendString(message);
    myDelay(500);
}

void bluetooth_setup(){
    sendMessage("AT+NAMEIcarus");
    sendMessage("");
    sendMessage("AT+PIN4011");
    sendMessage("");
    sendMessage("AT+BAUD7");
    sendMessage("");
}