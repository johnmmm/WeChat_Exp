#include "config.h"

//we send the alert message here~

void sendAlert(char header[], int target)
{
    strcpy(message_to_send, header);
    send(target , message_to_send , strlen(message_to_send) , 0);
}

void sendIt(int target, int size)
{
    printf("size: %d\n", size);
    send(target , message_to_send , size , 0);
}