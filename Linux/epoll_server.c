#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}
int main(int argc, char *argv[])
{
    int srvSock, clntSock;
    struct sockaddr_in srvAddr, clntAddr;
    socklen_t szAddr;
    int epfd, readCnt;
    char buff[100];

    if (argc != 2)
    {
        printf("Usage %s <PORT>\n", argv[0]);
        exit(1);
    }

    srvSock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(atoi(argv[1]));

    if (bind(srvSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
        ErrorHandling("bind() error!");
    if (listen(srvSock, 5) == -1)
        ErrorHandling("listen error!");

    epfd = epoll_create(50);
    struct epoll_event *epoll_events = malloc(sizeof(struct epoll_event) * 50);
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = srvSock;

    epoll_ctl(epfd, EPOLL_CTL_ADD, srvSock, &event);
    int epoll_cnt;
    while (1)
    {
        epoll_cnt = epoll_wait(epfd, epoll_events, 50, -1);
        if (epoll_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }
        for (int i = 0; i < epoll_cnt; i++)
        {
            if (epoll_events[i].data.fd == srvSock)
            {
                szAddr = sizeof(clntAddr);
                clntSock = accept(srvSock, (struct sockaddr *)&clntAddr, &szAddr);
                event.data.fd = clntSock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clntSock, &event);
                printf("connected client: %d\n", clntSock);
            }
            else
            {
                readCnt = read(epoll_events[i].data.fd, buff, sizeof(buff));
                if (readCnt == 0)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, epoll_events[i].data.fd, NULL);
                    close(epoll_events[i].data.fd);
                    printf("closed client: %d \n", epoll_events[i].data.fd);
                }
                else
                {
                    buff[readCnt] = 0;
                    write(epoll_events[i].data.fd, buff, strlen(buff));
                }
            }
        }
    }
    close(epfd);
    close(srvSock);
    return 0;
}