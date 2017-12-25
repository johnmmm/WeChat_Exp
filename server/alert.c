#include "config.h"

//we send the alert message here~

void sendAlert(char header[], int target)
{
    strcpy(message_to_send, header);
    send(target , message_to_send , sizeof(message_to_send) , 0);
}