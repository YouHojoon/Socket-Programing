#include<stdio.h>
#include<WinSock2.h>
void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET srvSock;
	SOCKADDR_IN srvAddr,clntAddr;
	int szClntAddr;
	char message[100];
	int strLen;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	srvSock=socket(PF_INET, SOCK_DGRAM, 0);
	if(srvSock==INVALID_SOCKET)
		ErrorHandling("UDP socket creation error");

	memset(&srvAddr, 0, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	srvAddr.sin_port = htons(atoi(argv[1]));

	if(bind(srvSock,(SOCKADDR *)&srvAddr, sizeof(srvAddr))==SOCKET_ERROR)
		ErrorHandling("bind() error");

	while (1) {
		szClntAddr = sizeof(szClntAddr);
		strLen = recvfrom(srvSock, message, sizeof(message), 0, (SOCKADDR*)& clntAddr, &szClntAddr);
		sendto(srvSock, message, strLen, 0, (SOCKADDR*)& clntAddr, &szClntAddr);
	}
	closesocket(srvSock);
	WSACleanup;
}
