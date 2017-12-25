#include "config.h"

void* handleRequest(int *fd)
{
    printf("begin to handle!!\n");
    int tmp_fd, ret;
    int who_am_i = -1;
    tmp_fd = *fd;
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
                strncpy(input[count], result, 999); 
                count++;
                result = strtok(NULL, delims);
            }
            printf("input: %s\n", input[0]);
            switch(input[0][0])
            {
                case REGISTER:
                    printf("wang to regist!\n");
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
                        sendAlert(USERNAME_UNAVAILABLE, tmp_fd);
                    }
                    else if(flag == 0)
                    {
                        if(new_user == -1)              //full
                        {
                            printf("users full\n");
                            sendAlert(USERS_FULL, tmp_fd);
                        }
                        else
                        {
                            printf("register success!\n");
                            who_am_i = new_user;
                            user_login[who_am_i] = 1;
                            user_socket[who_am_i] = tmp_fd;
                            strcpy(users[new_user].userName, input[1]);
                            strcpy(users[new_user].password, input[2]);
                            sendAlert(REGISTER_SUCCESS, tmp_fd);
                            refreshTxt();
                        } 
                    }
                    break;
                case LOGIN:
                    printf("wat to login\n");
                    printf("username: %s\n", input[1]);
                    printf("password: %s\n", input[2]);
                    int user_id = searchUsername(input[1]);
                    if(user_id >= 0)
                    {
                        if(strcmp(users[user_id].password, input[2]) != 0)
                        {
                            printf("password is wrong!!!\n");
                            sendAlert(WRONG_PASSWORD, tmp_fd);
                        }
                        else if(user_login[user_id] == 1)
                        {
                            printf("already login\n");
                            sendAlert(ALREADY_LOGIN, tmp_fd);
                        }
                        else if(strcmp(users[user_id].password, input[2]) == 0)
                        {
                            printf("password is right!!!\n");
                            who_am_i = user_id;
                            user_login[who_am_i] = 1;
                            user_socket[who_am_i] = tmp_fd; 
                            //sendAlert(LOGIN_SUCCESS, tmp_fd);
                            //send those message when offline
                            memset(message_to_send, 0, MAX_DATA_SIZE);
                            strcpy(message_to_send, CACHE_MESSAGE);
                            int place = 1;
                            message_to_send[place++] = 32;
                            message_to_send[place++] = 46;
                            message_to_send[place++] = 46;
                            message_to_send[place++] = 46;
                            message_to_send[place++] = 46;
                            message_to_send[place++] = 10;
                            printf("cache: %d\n", cache[who_am_i].size);
                            for(int i = 0; i < cache[who_am_i].size; i++)
                            {
                                message_to_send[place++] = cache[who_am_i].message[i];
                            }
                            send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                        } 
                    }
                    else if(user_id == -1)
                    {
                        printf("no such user!!\n");
                        sendAlert(WRONG_USERNAME, tmp_fd);
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
                        sendAlert(NOT_LOGIN, tmp_fd);
                        break;
                    }
                    int target_user = searchUsername(input[1]);
                    if(target_user == -1)
                    {
                        printf("target to a fake person!!!\n");
                        sendAlert(FAKE_PERSON, tmp_fd);
                        break;
                    }
                    else if(target_user >= 0)
                    {
                        if(is_friend(target_user, who_am_i) == 0)
                        {
                            printf("target is not your friend!!!\n");
                            sendAlert(NOT_FRIEND, tmp_fd);
                        }
                        else if(user_login[target_user] != 1)
                        {
                            printf("target not online!!!\n");
                            sendAlert(NOT_ONLINE, tmp_fd);

                            int mess_place = cache[target_user].size;
                            for(int i = 0; i < sizeof(users[who_am_i].userName); i++)
                            {
                                cache[target_user].message[mess_place++] = users[who_am_i].userName[i];
                            }
                            cache[target_user].message[mess_place++] = 58;
                            //cache[target_user].message[mess_place++] = 32;
                            for(int i = 0; i < sizeof(input[2]); i++)
                            {
                                cache[target_user].message[mess_place++] = input[2][i];
                            }
                            cache[target_user].message[mess_place++] = 10;
                            cache[target_user].size = mess_place;
                        }
                        else
                        {
                            printf("ready to send a message\n");
                            strcpy(message_to_send, SUCCESS);
                            message_to_send[1] = 32;
                            int place = 2;
                            for(int i = 0; i < sizeof(users[who_am_i].userName); i++)
                            {
                                message_to_send[place++] = users[who_am_i].userName[i];
                            }
                            message_to_send[place++] = 32;
                            for(int i = 0; i < sizeof(input[2]); i++)
                            {
                                message_to_send[place++] = input[2][i];
                            }
                            send(user_socket[target_user], message_to_send, sizeof(message_to_send), 0);
                        }
                    }
                    break;
                case ASK_FRIEND_LIST:
                    printf("want to gain friends list!\n");
                    if(who_am_i == -1)
                    {
                        printf("Not login!!!\n");
                        sendAlert(NOT_LOGIN, tmp_fd);
                        break;
                    }

                    strcpy(message_to_send, CHECK_FRIEND);
                    int place = 1;
                    for(int i = 0; i < FRIENDNUM; i++)
                    {
                        if(is_friend(searchUsername(users[who_am_i].friend_list[i]), who_am_i))
                        {
                            message_to_send[place++] = 32;
                            for(int j = 0; j < sizeof(users[who_am_i].friend_list[i]); j++)
                            {
                                message_to_send[place++] = users[who_am_i].friend_list[i][j];
                            }      
                        }
                    }
                    send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                    break;
                case ASK_FRIEND_ONLINE:
                    printf("want to know friends who are online!!\n");
                    if(who_am_i == -1)
                    {
                        printf("Not login!!!\n");
                        sendAlert(NOT_LOGIN, tmp_fd);
                        break;
                    }

                    strcpy(message_to_send, CHECK_FRIEND);
                    place = 1;
                    for(int i = 0; i < FRIENDNUM; i++)
                    {
                        if(is_friend(searchUsername(users[who_am_i].friend_list[i]), who_am_i) && user_login[searchUsername(users[who_am_i].friend_list[i])] == 1)
                        {
                            message_to_send[place++] = 32;
                            for(int j = 0; j < sizeof(users[who_am_i].friend_list[i]); j++)
                            {
                                message_to_send[place++] = users[who_am_i].friend_list[i][j];
                            }      
                        }
                    }
                    send(tmp_fd , message_to_send , sizeof(message_to_send) , 0);
                    break;
                case FILE_MESSAGE:
                    printf("want to send a file\n");
                    if(who_am_i == -1)
                    {
                        printf("Not login!!!\n");
                        sendAlert(NOT_LOGIN, tmp_fd);
                        break;
                    }
                    
                    break;
                case FRIEND_REQUEST:
                    printf("want to be friends\n");
                    if(who_am_i == -1)
                    {
                        printf("Not login!!!\n");
                        sendAlert(NOT_LOGIN, tmp_fd);
                        break;
                    }

                    int friend_id = searchUsername(input[1]);
                    if(friend_id == -1)
                    {
                        printf("no such user!!!\n");
                        sendAlert(FAKE_PERSON, tmp_fd);
                        break;
                    }
                    else if(friend_id >= 0)
                    {
                        printf("find it\n");
                        int whether_friend = (is_friend(who_am_i, friend_id) && is_friend(friend_id, who_am_i));
                        if(whether_friend == 0)
                        {
                            printf("not friend.\n");
                            flag = 0;
                            for(int i = 0; i < FRIENDNUM; i++)
                            {
                                printf("user: %s\n", users[user_id].friend_list[i]);
                                if(users[user_id].friend_list[i][0] == 0)
                                {
                                    flag = i;
                                    break;
                                }
                            }
                            strcpy(users[user_id].friend_list[flag], users[friend_id].userName);//jia de!!!!
                            printf("finish save it: %s\n", users[user_id].friend_list[flag]);
                            refreshTxt();

                            //offline save it
                            if(user_login[friend_id] == 0)//target is not online
                            {
                                sendAlert(NOT_ONLINE, tmp_fd);
                                int mess_place = cache[friend_id].size;
                                char messa[50];
                                strcpy(messa, "New_friend:");
                                for(int i = 0; i < sizeof(messa); i++)
                                {
                                    cache[friend_id].message[mess_place++] = messa[i];
                                }
                                for(int i = 0; i < sizeof(users[user_id].userName); i++)
                                {
                                    cache[friend_id].message[mess_place++] = users[user_id].userName[i];
                                }
                                cache[friend_id].message[mess_place++] = 10;
                                cache[friend_id].size = mess_place;
                                break;
                            }

                            strcpy(message_to_send, NEW_FRIEND);
                            message_to_send[1] = 32;
                            int place = 2;
                            for(int i = 0; i < sizeof(input[1]); i++)
                            {
                                message_to_send[place++] = input[1][i];
                            }
                            send(user_socket[friend_id] , message_to_send , sizeof(message_to_send) , 0);
                        }
                        else if(whether_friend == 1)
                        {
                            printf("already friend.\n");
                            sendAlert(ALREADY_FRIEND, tmp_fd);
                            break;
                        }
                    }
                    break;
                case DELETE_FRIEND:
                    printf("want to delete a friend!!!\n");
                    if(who_am_i == -1)
                    {
                        printf("Not login!!!\n");
                        sendAlert(NOT_LOGIN, tmp_fd);
                        break;
                    }

                    friend_id = searchUsername(input[1]);
                    if(friend_id == -1)
                    {
                        printf("no such user!!!\n");
                        sendAlert(FAKE_PERSON, tmp_fd);
                        break;
                    }

                    int whe_friend = is_friend(who_am_i, friend_id);
                    if(whe_friend == 0)
                    {
                        printf("It is not your friend!!!\n");
                        sendAlert(NOT_FRIEND, tmp_fd);
                    }
                    else if(whe_friend == 1)
                    {
                        printf("begin to delete it!!!\n");
                        int that_id = 0;
                        char blankchar[32] = {0};
                        for(int i = 0; i < FRIENDNUM; i++)
                        {
                            if(strcmp(users[user_id].friend_list[i], users[friend_id].userName) == 0)
                            {
                                memset(users[user_id].friend_list[i], 0, sizeof(users[user_id].friend_list[i]));
                            }
                        }
                        refreshTxt();
                        sendAlert(DELETE_SUCCESS, tmp_fd);
                    }
                    break;
                case LOGOUT:
                    printf("logout~~ \n");
                    if(who_am_i == -1)
                    {
                        printf("Not login!!!\n");
                        sendAlert(NOT_LOGIN, tmp_fd);
                        break;
                    }

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