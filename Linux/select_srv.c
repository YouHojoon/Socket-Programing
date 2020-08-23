#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}
int main(int argc, char* argv[]){
    int srvSock, clntSock;
    char buff[100];
    struct sockaddr_in srvAddr,clntAddr;
    socklen_t szAddr;
    fd_set reads,cpy_reads;
    struct timeval timeout;
    int strLen;
    int fdMax;

    if(argc!=2){
        printf("Usage %s <PORT>\n",argv[0]);
        exit(1);
    }

    memset(&srvAddr,0,sizeof(srvAddr));
    srvAddr.sin_family=AF_INET;
    srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    srvAddr.sin_port=htons(atoi(argv[1]));

    srvSock=socket(PF_INET,SOCK_STREAM,0);

    if(bind(srvSock,(struct sockaddr*)&srvAddr,sizeof(srvAddr))==-1)
        ErrorHandling("bind() error!");
    if(listen(srvSock,5)==-1)
        ErrorHandling("listen() error!");
    
    FD_ZERO(&reads);
    FD_SET(srvSock,&reads);
    fdMax=srvSock;
    int fdNum;
    int i;
    while(1){
        cpy_reads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;

        if((fdNum=select(fdMax+1,&cpy_reads,0,0,&timeout))==-1)//수신된 데이터가 있는지 확인 and 오류시
            break;

        if(fdNum==0)
            continue;
        for(i=0; i<fdMax+1; i++){
            
            if(FD_ISSET(i,&cpy_reads)){
                if(i==srvSock){//연결 요청이 들어왔을 때
                    szAddr=sizeof(clntSock);
                    clntSock=accept(srvSock,(struct sockaddr*)&clntSock,&szAddr);
                    FD_SET(clntSock,&reads);
                    if(fdMax<clntSock)
                        fdMax=clntSock;
                    printf("connected client: %d \n", clntSock);
                }
                else{//클라이언트에서 메시지가 날아왔을 때
                    strLen=read(i,buff,sizeof(buff));
                    if(strLen==0){//연결 종료
                        FD_CLR(i,&reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    }
                    else//echo
					{
						write(i,buff,strLen);
					}
                }
            }
        }
    }
    close(srvSock);
    return 0;
}