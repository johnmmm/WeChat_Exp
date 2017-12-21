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

#define SIN_PORT 7474
#define BACKLOG 20
#define MAX_DATA_SIZE 4096
#define MAX_NUM 20

struct Users
{
    char username[32];
    char password[32];
};

int main (int argc, char *argv[])
{
    struct sockaddr_in serverSockaddr,clientSockaddr;   //for addr
    int sockfd, recefd;                                 //for socket symbol
    int max_sockfd = 0, max_recefd = 0;                 //for max
    char username[32];                                  //username for server
    struct timeval timeout;                             //for refreshing
    socklen_t sin_size = 0;
    fd_set serverfd, clientfd;                          //for ids of server and client
    int client_num = 0;                                 //for counting number of client
    int server_sockfd[BACKLOG+1];                       //for saving the sockfds of server
    char client_name[BACKLOG+1][32];                    //for saving names of clients
    struct Users user;                                  //for recording user's information
    char sendBuf[MAX_DATA_SIZE], receBuf[MAX_DATA_SIZE];//for buffers 
    int on = 1;                                         //???

    if(argc != 2)                                       //command does not have right format
    {
        printf("username: ./server [username]\n");
        exit(1);
    }
    strcpy(username, argv[1]);
    printf("username: %s\n", username);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);           //try to build a socket(1, socket)
    if(sockfd == -1)
    {
        perror(" failed to build a socket!!\n");
        exit(1);
    }
    printf("success to build a socket!!!\n");

    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    serverSockaddr.sin_family = AF_INET;
    //serverSockaddr.sin_port = SIN_PORT;                 //configure my_addr
    serverSockaddr.sin_port = htons(SIN_PORT);
    //serverSockaddr.sin_addr.s_addr = INADDR_ANY;
    serverSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(serverSockaddr.sin_zero),8);
                                                        //bind it
    int bind_ans = bind(sockfd, (struct sockaddr*)&serverSockaddr, sizeof(struct sockaddr));
    if(bind_ans == -1)
    {
        perror( "failed to bind IP and socket!!!\n");
        exit(1);
    }
    printf("success to bind! \n");

    int listen_ans = listen(sockfd, BACKLOG);           //listen the server
    if(listen_ans == -1)
    {
        perror(" failed to listen server!!!\n");
        exit(1);
    }
    printf("success to listen!!!\n");

    timeout.tv_sec = 1;                                 //refresh after every 1s.
    timeout.tv_usec = 0;

    sin_size = sizeof(clientSockaddr);

    FD_ZERO(&serverfd);                                 //clean it 
    FD_ZERO(&clientfd);
    FD_SET(sockfd, &serverfd);

    client_num = 0;
    max_sockfd = sockfd;
    max_recefd = 0;

    memset(server_sockfd, 0, sizeof(server_sockfd));
    memset(client_name, 0, sizeof(client_name));

    int select_num = 0;
    while(1)
    {
        FD_ZERO(&serverfd);                                 //clean it 
        FD_ZERO(&clientfd);
        FD_SET(sockfd, &serverfd);
        
        select_num = select(max_sockfd+1, &serverfd, NULL, NULL, &timeout);
        //printf("select_num: %d\n", select_num);
        if(select_num == -1)
        {
            perror(" failed to select!!!\n");
            //exit(1);
            continue;
        }
        else if(select_num == 0)
        {
            //printf("select_num is 0!!\n");
            continue;
        }
        else
        {
            printf("else!!\n");
            if(FD_ISSET(sockfd, &serverfd))             //if there is something to 
            {
                recefd = accept(sockfd, (struct sockaddr*)&clientSockaddr, &sin_size);
                if(recefd == -1)                        //accept it
                {
                    perror(" failed to accept!!!\n");
                    exit(1);
                }
                printf("success to accept!! \n");
                printf(">>>>>> %s:%d join in! ID(fd):%d \n",inet_ntoa(clientSockaddr.sin_addr),ntohs(clientSockaddr.sin_port),recefd);

                int receSize = recv(recefd, &user, sizeof(struct Users), 0);
                if(receSize == -1 || receSize == 0)
                {
                    perror(" failed to gain data from client!!!\n");
                    exit(1);
                }
                printf("username: %s, password: %s", user.username, user.password);
                printf("Success! \n");
                strcpy(sendBuf,"yes");
                
                memset(receBuf, 0, sizeof(receBuf));
                int sendSize = send(recefd, sendBuf, MAX_DATA_SIZE, 0);
                if(sendSize == -1 || sendSize == 0)
                {
                    perror(" failed to send!!!\n");
                }
                server_sockfd[client_num] = recefd;
                strcpy(client_name[client_num], user.username);
                client_num++;
                max_recefd = recefd;
            }
        }

        for(int i = 0; i < MAX_NUM; i++)
        {
            if(server_sockfd[i] != 0)
            {
                FD_SET(server_sockfd[i], &clientfd);
            }
        }

        select_num = select(max_recefd+1, &clientfd, NULL, NULL, &timeout);
        if(select_num == -1)
        {
            continue;
        }
        else if(select_num == 0)
        {
            continue;
        }
        else
        {
            for(int i = 0; i < client_num; i++)
            {
                if(FD_ISSET(recefd, &clientfd))
                {

                }
            }
        }
    }

    return 0;
}