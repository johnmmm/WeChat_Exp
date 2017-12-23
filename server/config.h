#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define FILE_URL "/Users/mac/Desktop/programme/program/1718Autumn/WeChat_Exp/server/users/info.txt"

#define SIN_PORT 7474
#define BACKLOG 40
#define MAX_DATA_SIZE 40960
#define MAX_NUM 40
#define MAX_LINE  8192
#define FRIENDNUM 10
#define USERNUM 100
#define bufN 38400

//need to define those types of message!
#define REGISTER 'R'
#define LOGIN 'L'
#define HELP 'H'
#define CHAT_MESSAGE 'M'
#define ASK_FRIEND_LIST 'A'
#define ASK_FRIEND_ONLINE 'S'
#define FILE_MESSAGE 'F'
#define LOGOUT 'O'
#define UNKNOWN 'U'

//the type of message to return
#define EXCEED "exceed"
#define SUCCESS "success"
#define FAILED "failed"
#define USERNAME_UNAVAILABLE "U"
#define WRONG_USERNAME "wrong_username"
#define WRONG_PASSWORD "wrong_password"
#define ALREADY_ONLINE "already_online"
#define NOT_FRIEND "not_friend"
#define ALREADY_FRIEND "already_friend"
#define USERS_FULL "users_full"

typedef struct _User                    //To record those users
{
    char userName[32];
    char password[32];
    char friend_list[FRIENDNUM][32];    //friend list (maximum is 10)
}User;

typedef struct _User_Online             //To record the user online
{
    char userName[32];
    struct sockaddr_in userAddr;        //ip addr
    int sockfd;                         //record its sockfd~
}Users_Online;

typedef struct _ListNode
{
    Users_Online user_online;
    struct _ListNode *next;
}ListNode;

int user_num;
User users[USERNUM];
ListNode *userlist;              //record the user who are online

//chat.c
extern void* handleRequest(int *fd);

//info.c
extern void readTxt();
extern void refreshTxt();