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
            printf("logout!! \n");
            return NULL;
        }
        else
        {
            //deal with the buf
            char input[10][1000];
            char delims[] = " ";
            char *result = NULL;
            memset(input, 0, 10000);
            result = strtok(buf, delims);
            int count = 0;
            while(result != NULL)
            {
                //input[count] = (char)result;
                strncpy(input[count], result, 999); 
                count++;
                result = strtok(NULL, delims);
            }
            printf("input: %s\n", input[0]);
            switch(input[0][0])
            {
                case REGISTER:
                    printf("wang to regist!\n");
                    break;
                case LOGIN:
                    printf("username: %s\n", input[1]);
                    printf("password: %s\n", input[2]);
                    break;
                case HELP:
                    printf("want to get help!\n");
                    break;
                case CHAT_MESSAGE:
                    printf("want to send a message\n");
                    break;
                case ASK_FRIEND_LIST:
                    printf("want to gain friends list!\n");
                    break;
                case ASK_FRIEND_ONLINE:
                    printf("want to know friends who are online!!\n");
                    break;
                case FILE_MESSAGE:
                    printf("want to send a file");
                    break;
                case LOGOUT:
                    printf("logout~~ \n");
                    *fd = -1;
                    return NULL;
                case UNKNOWN:
                default:
                    printf("message: %s\n", buf);
                    printf("Unknown command!\n");
                    break;
            }
        }
        memset(buf, 0, MAX_LINE);
    }
    *fd = -1;
    return NULL;
}