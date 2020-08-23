#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void *send_msg(void *arg);
void *recv_msg(void *arg);
void error_handling(char *msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in srvAddr;
    pthread_t send_thread, rcv_thread;
    void *thread_return;

    if (argc != 4)
    {
        printf("Usage %s <IP> <PORT> <NAME>", argv[0]);
        exit(1);
    }
    sprintf(name, "[%s]", argv[3]);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(argv[1]);
    srvAddr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
        error_handling("connect() error");

    pthread_create(&send_thread, NULL, send_msg, (void *)&sock);
    pthread_create(&rcv_thread, NULL, recv_msg, (void *)&sock);
    pthread_join(send_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);
    close(sock);
    return 0;
}
void *send_msg(void *arg)
{
    int sock = *((int *)arg);
    char buff[NAME_SIZE + BUF_SIZE];
    while (1)
    {
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
        {
            shutdown(sock, SHUT_WR);
            exit(0);
        }
        sprintf(buff, "%s %s", name, msg);
        write(sock, buff, strlen(buff));
    }
}

void *recv_msg(void *arg)
{
    int sock = *((int *)arg);
    char buff[NAME_SIZE + BUF_SIZE];
    int strLen;
    while (1)
    {
        strLen = read(sock, buff, NAME_SIZE + BUF_SIZE - 1);
        if (strLen == -1)
            return (void *)-1;
        buff[strLen] = 0;
        fputs(buff, stdout);
    }
    return NULL;
}

void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}