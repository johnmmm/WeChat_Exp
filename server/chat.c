#include "config.h"

void* handleRequest(int *fd)
{
    printf("begin to handle!!\n");
    int tmp_fd, ret;
    int who_am_i = -1;
    tmp_fd = *fd;
    char message_to_send[MAX_DATA_SIZE];
    printf("tmp_fd: %d\n", tmp_fd);

    char buf[MAX_LINE];
    memset(buf, 0, MAX_LINE);
    memset(message_to_send, 0, MAX_DATA_SIZE);
    
    while(1)
    {
        memset(message_to_send, 0, MAX_DATA_SIZE);
        int rece_num = recv(tmp_fd, buf, sizeof(buf)+1, 0);
        printf("begin %d\n", rece_num);
        if(rece_num <= 0)
        {
            fflush(stdout);
            //close(stdout);
            *fd = -1;
            printf("logout!! \n");
            user_login[who_am_i] = 0;
            user_socket[who_am_i] = 0;
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
                    printf("user exist: %s\n", users[0].userName);
                    printf("username: %s\n", input[1]);
                    printf("password: %s\n", input[2]);
                    int flag = 0;
                    int new_user = -1;
                    for(int i = 0; i < USERNUM; i++)
                    {
                        if(users[i].userName[0] == 0)   //no username
                        {
                            new_user = i;
                            break;
                        }
                        if(strcmp(users[i].userName, input[1]) == 0)
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if(flag == 1)
                    {
                        printf("username_unavailable\n");
                        strcpy(message_to_send, USERNAME_UNAVAILABLE);
                        send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                    }
                    else if(flag == 0)
                    {
                        if(new_user == -1)              //full
                        {
                            printf("users full\n");
                            strcpy(message_to_send, USERS_FULL);
                            send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                        }
                        else
                        {
                            printf("register success!\n");
                            who_am_i = new_user;
                            user_login[who_am_i] = 1;
                            user_socket[who_am_i] = tmp_fd;
                            strcpy(users[new_user].userName, input[1]);
                            strcpy(users[new_user].password, input[2]);
                            refreshTxt();
                        } 
                    }
                    break;
                case LOGIN:
                    printf("wat to login\n");
                    printf("username: %s\n", input[1]);
                    printf("password: %s\n", input[2]);
                    flag = 0;
                    int user_id = -1;
                    for(int i = 0; i < USERNUM; i++)
                    {
                        if(strcmp(users[i].userName, input[1]) == 0)
                        {
                            flag = 1;
                            user_id = i;
                            break;
                        }
                    }
                    if(flag == 1)
                    {
                        if(strcmp(users[user_id].password, input[2]) != 0)
                        {
                            printf("password is wrong!!!\n");
                            strcpy(message_to_send, WRONG_PASSWORD);
                            send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                        }
                        else if(user_login[user_id] == 1)
                        {
                            printf("already login\n");
                            strcpy(message_to_send, ALREADY_LOGIN);
                            send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                        }
                        else if(strcmp(users[user_id].password, input[2]) == 0)
                        {
                            printf("password is right!!!\n");
                            who_am_i = user_id;
                            user_login[who_am_i] = 1;
                            user_socket[who_am_i] = tmp_fd; 
                            strcpy(message_to_send, LOGIN_SUCCESS);
                            send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                        } 
                    }
                    else if(flag == 0)
                    {
                        printf("no such user!!\n");
                        strcpy(message_to_send, WRONG_USERNAME);
                        send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                    }
                    break;
                case HELP:
                    printf("want to get help!\n");
                    break;
                case CHAT_MESSAGE:
                    printf("want to send a message\n");
                    if(who_am_i == -1)
                    {
                        printf("Not login!!!\n");
                        strcpy(message_to_send, NOT_LOGIN);
                        send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                        break;
                    }
                    flag = 0;
                    int target_user = -1;
                    for(int i = 0; i < USERNUM; i++)
                    {
                        if(strcmp(users[i].userName, input[1]) == 0)
                        {
                            flag = 1;
                            target_user = i;
                            break;
                        }
                    }
                    if(flag == 0)
                    {
                        printf("target to a fake person!!!\n");
                        strcpy(message_to_send, FAKE_PERSON);
                        send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                        break;
                    }
                    else if(flag == 1)
                    {
                        if(user_login[target_user] != 1)
                        {
                            printf("target not online!!!\n");
                            strcpy(message_to_send, NOT_ONLINE);
                            send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                        }
                        else
                        {
                            printf("ready to send a message\n");
                            strcpy(message_to_send, SUCCESS);
                            message_to_send[1] = 32;
                            int place = 2;
                            for(int i = 0; i < sizeof(input[1]); i++)
                            {
                                message_to_send[place++] = input[1][i];
                            }
                            message_to_send[place++] = 32;
                            for(int i = 0; i < sizeof(input[2]); i++)
                            {
                                message_to_send[place++] = input[2][i];
                            }
                            send(user_socket[target_user] , message_to_send , sizeof(message_to_send) , 0);
                        }
                    }

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
                    user_login[who_am_i] = 0;
                    user_socket[who_am_i] = 0;
                    who_am_i = -1;
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