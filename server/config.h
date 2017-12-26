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

#define FILE_URL "./info.txt"

#define SIN_PORT 7474
#define BACKLOG 40
#define MAX_DATA_SIZE 409600
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
#define FRIEND_REQUEST 'Q'
#define DELETE_FRIEND 'D'
#define FILE_REQUEST 'I'
#define LOGOUT 'O'
#define UNKNOWN 'U'

//the type of message to return
#define EXCEED "exceed"
#define SUCCESS "C"
#define FAILED "failed"
#define USERNAME_UNAVAILABLE "U"
#define REGISTER_SUCCESS "G"
#define LOGIN_SUCCESS "S"
#define WRONG_USERNAME "E"
#define WRONG_PASSWORD "P"
#define ALREADY_LOGIN "A"               //cannot be login in in another shell
#define NOT_FRIEND "I"
#define ALREADY_FRIEND "L"
#define USERS_FULL "F"
#define NOT_LOGIN "N"
#define NOT_ONLINE "O"                  //target person is not online
#define FAKE_PERSON "K"
#define NEW_FRIEND "Q"
#define CHECK_FRIEND "H"
#define DELETE_SUCCESS "D"
#define CACHE_MESSAGE "B"
#define FILE_MESSAGE "Z"


typedef struct _User                    //To record those users
{
    char userName[32];
    char password[32];
    char friend_list[FRIENDNUM][32];    //friend list (maximum is 10)
}User;

typedef struct _Cache
{
    int size;
    char message[MAX_DATA_SIZE];
}Cache;

int user_num;
int user_login[USERNUM];                //record the user who are online
int user_socket[USERNUM];
User users[USERNUM];     
Cache cache[USERNUM];       

//chat.c
char message_to_send[MAX_DATA_SIZE];
extern void* handleRequest(int *fd);

//info.c
extern int searchUsername(char username[]);
extern int is_friend(int user_id, int target_id);
extern void readTxt();
extern void refreshTxt();

//alert.c
extern void sendAlert(char header[], int target);