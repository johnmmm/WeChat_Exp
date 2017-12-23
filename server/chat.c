#include "config.h"

void* handleRequest(int *fd)
{
    printf("begin to handle!!\n");
    int tmp_fd, ret;
    tmp_fd = *fd;
    printf("tmp_fd: %d\n", tmp_fd);

    char buf[MAX_LINE];
    memset(buf, 0, MAX_LINE);
    
    while(1)
    {
        int rece_num = recv(tmp_fd, buf, sizeof(buf)+1, 0);
        printf("begin %d\n", rece_num);
        if(rece_num <= 0)
        {
            fflush(stdout);
            //close(stdout);
            *fd = -1;
            printf("logout~~ \n");
            return NULL;
        }
        else
        {
            if(buf[0] == 'o')
            {
                printf("logout~~ \n");
                *fd = -1;
                return NULL;
            }
            else
                printf("message: %s\n", buf);
        }
        memset(buf, 0, MAX_LINE);
    }
    *fd = -1;
    return NULL;
}