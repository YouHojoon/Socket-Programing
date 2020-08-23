#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

int main(int argc, char* argv[]){
    int sock,strLen;
    struct sockaddr_in srvAddr;
    struct ip_mreq join_group;
    char buff[100];
    if(argc!=3)
    {
        printf("Useage %s <IP> <PORT>\n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&srvAddr,0,sizeof(srvAddr));
    srvAddr.sin_family=AF_INET;
    srvAddr.sin_port=htons(atoi(argv[2]));
    srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(sock,(struct sockaddr*)&srvAddr,sizeof(srvAddr))==-1)
        ErrorHandling("bind() error!");
    
    join_group.imr_multiaddr.s_addr=inet_addr(argv[1]);
    join_group.imr_interface.s_addr=htonl(INADDR_ANY);

    setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(void*)&join_group,sizeof(join_group));

    while(1){
        strLen=recvfrom(sock,buff,sizeof(buff),0,NULL,0);
        if(strLen<0)
            break;
        buff[strLen]=0;
        fputs(buff, stdout);
    }
    close(sock);
    return 0;
}