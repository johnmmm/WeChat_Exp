#include "config.h"

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
    pthread_t client_thread;
    fd_set serverfd, clientfd;                          //for ids of server and client
    int client_num = 0;                                 //for counting number of client
    int server_sockfd[FD_SETSIZE+1];                    //for saving the sockfds of server
    char client_name[FD_SETSIZE+1][32];                 //for saving names of clients
    struct Users user;                                  //for recording user's information
    char sendBuf[MAX_DATA_SIZE], receBuf[MAX_DATA_SIZE];//for buffers 
    int on = 1;                                         //???

    readTxt(username);
    //refreshTxt(username);

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
                                                        //configure my_addr
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));               
    serverSockaddr.sin_port = htons(SIN_PORT);
    serverSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
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

    memset(server_sockfd, -1, sizeof(server_sockfd));
    memset(client_name, 0, sizeof(client_name));

    int select_num = 0;
    while(1)
    {
        clientfd = serverfd;
        //printf("begin to ...\n");
        select_num = select(max_sockfd+1, &clientfd, NULL, NULL, &timeout);
        printf("select_num: %d\n", select_num);
        //printf("select_num: %d\n", select_num);
        if(select_num == -1)
        {
            perror(" failed to select!!!\n");
            //exit(1);
            continue;
        }
        else if(select_num == 0)
        {
            printf("select_num is 0!!\n");
        }
        else
        {
            printf("else!! %d\n", sockfd);
            if(FD_ISSET(sockfd, &clientfd))             //if there is something to 
            {
                recefd = accept(sockfd, (struct sockaddr*)&clientSockaddr, &sin_size);
                printf("????\n");
                if(recefd == -1)                        //accept it
                {
                    perror(" failed to accept!!!\n");
                    exit(1);
                }
                printf("success to accept!! \n");
                printf(">>>>>> %s:%d join in! ID(fd):%d \n",inet_ntoa(clientSockaddr.sin_addr),ntohs(clientSockaddr.sin_port),recefd);

                int i = 0;
                for(i = 0; i < FD_SETSIZE; i++)
                {
                    if(server_sockfd[i] < 0)
                    {
                        printf("recefd %d is %d\n", i, recefd);
                        server_sockfd[i] = recefd;
                        break;
                    }
                }

                if(i == FD_SETSIZE)
                {
                    perror("here are too many connnections!!\n");
                    exit(1);
                }

                //put num in accept into serverfd to say that it is going to be handled
                FD_SET(recefd, &serverfd);

                //refresh the max one
                if(recefd > max_recefd)
                    max_recefd = recefd;
                if(i > max_recefd)
                    max_recefd = i;
                printf("finish refreshing!!!\n");
                printf("the select num is %d\n", select_num);
                if(--select_num <= 0)
                    continue;
                //maybe there is a problem about select_num
                //cannot understand function select very well now..
            }
        }

        //and then we handle all those sockets
        int tmp_fd = 0;
        for(int i = 0; i <= max_recefd; i++)
        {
            //printf("now handle No.%d, socket is %d\n", i, server_sockfd[i]);
            if(server_sockfd[i] < 0)
            {
                continue;
            }
            tmp_fd = server_sockfd[i];
            printf("can we handle it?\n");
            if(FD_ISSET(tmp_fd, &serverfd))
            {
                //if??
                printf("can we create it?\n");
                if(select_num < 0)
                    break;
                printf("begin to handle it. tmp_fd: %d\n", tmp_fd);
                pthread_create(&client_thread, NULL, (void *)handleRequest, (void*)&tmp_fd);
                printf("finish handling it. tmp_fd: %d\n", tmp_fd);
                FD_CLR(tmp_fd, &serverfd);
                server_sockfd[i] = -1;
            }
        }
    }
    return 0;
}