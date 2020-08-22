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
	SOCKET srvSock, clntSock;
	SOCKADDR_IN srvAddr, clntAddr;
	int szClntAddr;
	char buff[100];
	int readCnt;
	FILE* fp;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	srvSock = socket(PF_INET,SOCK_STREAM,0);
	if (srvSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	if(fopen_s(&fp,"C:\\Users\\N\\Desktop\\test.txt", "rb")!=0)
		ErrorHandling("fopen_s() error!");

	memset(&srvAddr, 0, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvAddr.sin_port = htons(atoi(argv[1]));

	if (bind(srvSock, (SOCKADDR*)& srvAddr, sizeof(srvAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");

	if (listen(srvSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	szClntAddr = sizeof(clntAddr);
	clntSock = accept(srvSock, (SOCKADDR*)& clntAddr, &szClntAddr);

	while (1) {
		readCnt = fread((void*)buff, 1, sizeof(buff), fp);
		if (readCnt < sizeof(buff)) {
			send(clntSock, (char*)buff, readCnt, 0);
			break;
		}
		send(clntSock, (char*)buff, sizeof(buff), 0);
	}
	shutdown(clntSock, SD_SEND);//출력 스트림 종료

	recv(clntSock, (char*)buff, sizeof(buff), 0);
	printf("Message from client: %s \n", buff);

	fclose(fp);
	closesocket(clntSock);
	closesocket(srvSock);
	WSACleanup();
	return 0;
}