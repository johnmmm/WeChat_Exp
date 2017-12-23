#include "config.h"

//we save the informations of users here
char buf[bufN];
char *p = buf;


void readTxt()
{
    FILE* stream;
    if((stream = fopen("/Users/mac/Desktop/programme/program/1718Autumn/WeChat_Exp/server/info.txt", "r+")) == NULL)
    {
        printf("cannot open the txt file!!!\n");
        return;
    }
    
    fread(buf, 1, bufN, stream);
    printf("text content: %s\n", buf);

    //put those data into buf

    return;
}

void refreshTxt()
{
    FILE* stream;
    if((stream = fopen("/Users/mac/Desktop/programme/program/1718Autumn/WeChat_Exp/server/info.txt", "w+")) == NULL)
    {
        printf("cannot open the txt file!!!\n");
        return;
    }

    //change those data into buf

    fwrite(buf, 1, bufN, stream);
    printf("put data into files\n");

    return;
}