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

#define SIN_PORT 7474
#define BACKLOG 40
#define MAX_DATA_SIZE 4096
#define MAX_NUM 40
#define MAX_LINE  8192

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
#define EXCEED 20
#define SUCCESS 21
#define FAILED 22
#define USERNAME_UNAVAILABLE 23
#define WRONG_USERNAME 24
#define WRONG_PASSWORD 25
#define ALREADY_ONLINE 26

extern void* handleRequest(int *fd);