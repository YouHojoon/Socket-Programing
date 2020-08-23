#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<signal.h>

#define BUFF_SIZE 100
void errorHandling(char* message);
void readChildProc(int signal);
int main(int argc, char * argv[]){
    int srvSock, clntSock;
    struct sockaddr_in srvAddr, clntAddr;
    socklen_t szAddr;
    pid_t pid;
    int fds[2];
    struct sigaction act;
    int state, readCnt;
    char buff[BUFF_SIZE];

    if(argc!=2){
        printf("Usage %s <PORT>\n",argv[0]);
        exit(1);
    }

    sigemptyset(&act.sa_mask);
    act.sa_handler=readChildProc;
    state=sigaction(SIGCHLD,&act,0);

    memset(&srvAddr,0,sizeof(srvAddr));
    srvAddr.sin_family=AF_INET;
    srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    srvAddr.sin_port=htons(atoi(argv[1]));

    srvSock=socket(PF_INET,SOCK_STREAM,0);
    if(bind(srvSock,(struct sockaddr*)&srvAddr,sizeof(srvAddr))==-1)
        errorHandling("bind() error!");
    if(listen(srvSock,5)==-1)
        errorHandling("listen() error!");
    
    pipe(fds);
    pid=fork();

    if(pid==0){
        FILE *fp=fopen("echostore.txt","w");
        char msgBuff[BUFF_SIZE];
        int len;
        for(int i=0; i<10; i++){
            len=read(fds[0],msgBuff,BUFF_SIZE);
            fwrite(msgBuff,1,len,fp);
        }
        fclose(fp);
        return 0;
    }

    while(1){
        szAddr=sizeof(clntAddr);
        clntSock=accept(srvSock,(struct sockaddr*)&clntAddr, &szAddr);
        if(clntSock==-1)
            continue;
        else
            puts("New Client Connected...");
        pid=fork();

        if(pid==0){
            close(srvSock);
            while((readCnt=read(clntSock,buff,BUFF_SIZE))!=0){
                write(clntSock,buff,readCnt);
                write(fds[1],buff,readCnt);
            }
            close(clntSock);
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
void errorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
void readChildProc(int signal){
    pid_t pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG);
    printf("remove pid: %d\n",pid);
}