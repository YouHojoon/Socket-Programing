#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFF_SIZE 100
void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}
void writeRoutine(int socket, char *buff)
{
    while (1)
    {
        fgets(buff, BUFF_SIZE, stdin);
        if (!strcmp(buff, "q\n") || !strcmp(buff, "Q\n"))
        {
            shutdown(socket, SHUT_WR);
            return;
        }
        write(socket,buff,strlen(buff));
    }
}
void readRoutine(int socket, char *buff)
{
    int readCnt;
    while (1)
    {
        readCnt=read(socket, buff, BUFF_SIZE);
        if(readCnt==0)
            return;
        buff[readCnt]=0;
        printf("Message from Server: %s",buff);
    }
}

int main(int argc, char *argv[])
{
    int s;
    pid_t pid;
    char buff[BUFF_SIZE];
    struct sockaddr_in srvAddr;

    if (argc != 3)
    {
        printf("Useage %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    s = socket(PF_INET, SOCK_STREAM, 0);

    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(argv[1]);
    srvAddr.sin_port = htons(atoi(argv[2]));

    if (connect(s, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
        ErrorHandling("connect() error!");

    pid = fork();

    if (pid == 0)
        writeRoutine(s,buff);
        //write_routine(s,buff);
    else
        //read_routine(s,buff); 
        readRoutine(s,buff);

    close(s);
    return 0;
}