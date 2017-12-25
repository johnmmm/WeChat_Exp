#include "config.h"

//search users' information here
int searchUsername(char username[])
{
    int user_id = -1;
    char zero_name[32] = {0};
    if(strcmp(zero_name, username) == 0)
    {
        return user_id;//fake person
    }
    for(int i = 0; i < USERNUM; i++)
    {
        if(strcmp(users[i].userName, username) == 0)
        {
            printf("in: %d\n", i);
            user_id = i;
            break;
        }
        printf("%d\n", i);
    }
    return user_id;//if -1 then no such user; else if > 0 then it is the user_id
}

int is_friend(int user_id, int target_id)
{
    int flag = 0;
    if(user_id < 0 || target_id < 0 || user_id >= USERNUM || target_id >= USERNUM)
    {
        return flag;
    }
    for(int i = 0; i < FRIENDNUM; i++)
    {
        if(strcmp(users[user_id].friend_list[i], users[target_id].userName) == 0)
        {
            flag = 1;
        }
    }
    return flag;//if 0 then not friend else if 1 then is friend
}

//we save the informations of users here

void readTxt()
{
    FILE* stream;
    if((stream = fopen(FILE_URL, "r+")) == NULL)
    {
        printf("cannot open the txt file!!!\n");
        return;
    }
    
    char buf[bufN];
    char *p = buf;
    fread(buf, 1, bufN, stream);

    memset(users, 0, sizeof(users));

    //put those data into buf
    int count = 0;
    while(count < 38400)
    {
        for(int i = 0; i < 100; i++)
        {
            for(int j = 0; j < 32; j++)
            {
                if(buf[count] != 32)
                    users[i].userName[j] = buf[count];
                count++;
            }
            for(int j = 0; j < 32; j++)
            {
                if(buf[count] != 32)
                    users[i].password[j] = buf[count];
                count++;
            }
            for(int j = 0; j < FRIENDNUM; j++)
            {
                for(int k = 0; k < 32; k++)
                {
                    if(buf[count] != 32)
                        users[i].friend_list[j][k] = buf[count];
                    count++;
                }
            }
        }
    }
    printf("finishing reading!!\n");
    // printf("the first username: %s\n", users[0].userName);
    // printf("the first password: %s\n", users[0].password);
    // printf("the first friend: %s\n", users[0].friend_list[0]);

    return;
}

void refreshTxt()
{
    FILE* stream;
    if((stream = fopen(FILE_URL, "w+")) == NULL)
    {
        printf("cannot open the txt file!!!\n");
        return;
    }

    // strcpy(users[0].userName, "John_WJs");
    // strcpy(users[0].password, "John_WJs");
    // strcpy(users[0].friend_list[0], "damaoyu");

    char buf[bufN];
    char *p = buf;
    //change those data into buf
    int count = 0;
    while(count < 38400)
    {
        for(int i = 0; i < 100; i++)
        {
            for(int j = 0; j < 32; j++)
            {
                if(users[i].userName[j] != 0)
                    buf[count++] = users[i].userName[j];
                else
                    buf[count++] = 32;
            }
            for(int j = 0; j < 32; j++)
            {
                if(users[i].password[j] != 0)
                    buf[count++] = users[i].password[j];
                else
                    buf[count++] = 32;
            }
            for(int j = 0; j < FRIENDNUM; j++)
            {
                for(int k = 0; k < 32; k++)
                {
                    if(users[i].friend_list[j][k] != 0)
                        buf[count++] = users[i].friend_list[j][k];
                    else
                        buf[count++] = 32;
                }
            }
        }
    }
    //printf("data is like this: %s\n", buf);
    fwrite(buf, 1, bufN, stream);
    printf("put data into files\n");

    return;
}