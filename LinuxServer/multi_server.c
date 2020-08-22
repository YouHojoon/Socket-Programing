#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void ErrorHandling(char *message);
void read_childProc(int signal);

int main(int argc, char *argv[])
{
    pid_t pid;
    int srvSock, clntSock;
    struct sockaddr_in srvAddr, clntAddr;
    struct sigaction act;
    socklen_t szAddr;
    int strLen, state;
    char buff[100];

    if (argc != 2)
    {
        printf("Usage %s <port>", argv[0]);
        exit(1);
    }
    act.sa_handler = read_childProc;//시그널 발생 시 실행할 함수
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    state = sigaction(SIGCHLD, &act, 0);
    srvSock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family=AF_INET;
    srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    srvAddr.sin_port=htons(atoi(argv[1]));

    if(bind(srvSock,(struct sockaddr*)&srvAddr,sizeof(srvAddr))==-1)
        ErrorHandling("bind() error!");

    if(listen(srvSock,5)==-1)
        ErrorHandling("listen() error!");

    while(1){
        szAddr=sizeof(clntAddr);
        clntSock=accept(srvSock,(struct sockaddr*)&clntAddr,&szAddr);
        if(clntSock==-1)
            continue;
        else
            puts("New Client Connected---");

        pid=fork();//새 프로세스 생성

        if(pid==-1){
            close(clntSock);
            continue;
        }
        if(pid==0){//자식 프로세스 일 경우
            
            close(srvSock);//자식 프로세스의 소켓도 닫아야한다. 
            
            while((strLen=read(clntSock,buff,sizeof(buff)))!=0)
                write(clntSock,buff,strLen);
            
            close(clntSock);
            puts("Client Disconnet");
            return 0;
        }
        else
        {
            close(clntSock);
        }
    } 
    close(srvSock);
    return 0;   

}
void ErrorHandling(char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}
void read_childProc(int signal){
    pid_t pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG);//자식 프로세스가 끝날 때 까지 대기
    printf("remove proc id: %d \n", pid);
}