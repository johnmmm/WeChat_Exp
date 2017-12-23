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

//need to define those types of message!
#define REGISTER 1
#define LOGIN 2
#define HELP 3
#define CHAT_REQUEST 4
#define CHAT_MESSAGE 5
#define ASK_FRIEND_LIST 6
#define FILE_MESSAGE 7
#define UNKNOWN 8

//the type of message to return
#define EXCEED 20
#define SUCCESS 21
#define FAILED 22
#define USERNAME_UNAVAILABLE 23
#define WRONG_USERNAME 24
#define WRONG_PASSWORD 25
#define ALREADY_ONLINE 26