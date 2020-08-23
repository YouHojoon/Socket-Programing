#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}
int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in srvAddr;
    int timelive;
    int bcast=1;
    char buff[100];
    if(argc!=3){
        printf("Useage %s <IP> <PORT>",argv[0]);
        exit(1);
    } 

    sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&srvAddr,0,sizeof(srvAddr));
    srvAddr.sin_family=AF_INET;
    srvAddr.sin_addr.s_addr=inet_addr(argv[1]);
    srvAddr.sin_port=htons(atoi(argv[2]));

    setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(void*)&bcast,sizeof(bcast));
    FILE *fp;
    if((fp=fopen("news.txt","r"))==NULL)
        ErrorHandling("fopen() error!");
    while(!feof(fp)){
        fgets(buff,sizeof(buff),fp);
        sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&srvAddr,sizeof(srvAddr));
        sleep(2);
    }
    fclose(fp);
    close(sock);
    return 0;    
}