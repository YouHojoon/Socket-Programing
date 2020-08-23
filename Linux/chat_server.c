#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handling(char *msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
    int srvSock, clntSock;
    struct sockaddr_in srvAddr, clntAddr;
    socklen_t szAddr;
    pthread_t t_id;

    if (argc != 2)
    {
        printf("Useage %s <PORT>\n", argv[0]);
        exit(1);
    }

    srvSock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(atoi(argv[1]));

    if (bind(srvSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
        error_handling("bind() error");
    if (listen(srvSock, 5) == -1)
        error_handling("listen() error");

    pthread_mutex_init(&mutx, NULL);
    while (1)
    {
        szAddr = sizeof(clntAddr);
        clntSock = accept(srvSock, (struct sockaddr *)&clntAddr, &szAddr);
        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clntSock;
        pthread_mutex_unlock(&mutx);
        pthread_create(&t_id, NULL, handle_clnt, (void *)&clntSock);

        pthread_detach(t_id);
        printf("Connected client IP: %s \n", inet_ntoa(clntAddr.sin_addr));
    }
    close(srvSock);
    return 0;
}
void *handle_clnt(void *arg)
{
    int clntSock = *((int *)arg);
    int strLen;
    char buff[BUF_SIZE];

    while ((strLen = read(clntSock, buff, BUF_SIZE)) != 0)
        send_msg(buff, strLen);

    pthread_mutex_lock(&mutx);
    for (int i = 0; i < clnt_cnt; i++)
    {
        if (clnt_socks[i] == clntSock)
        {
            while (i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);
    close(clntSock);
    return NULL;
}
void send_msg(char *msg, int len)
{
    pthread_mutex_lock(&mutx);
    for (int i = 0; i < clnt_cnt; i++)
        write(clnt_socks[i], msg, len);
    pthread_mutex_unlock(&mutx);
}
void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}