//#include<stdio.h>
//#include<WinSock2.h>
//#include<WS2tcpip.h>
//
//void ErrorHandling(char* message)
//{
//	fputs(message, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}
//
//int main(int argc, char* argv[]) {
//	WSADATA wsaData;
//	SOCKET s;
//	SOCKADDR_IN srvAddr;
//	int readCnt;
//	FILE* fp;
//	char buff[100];
//
//	if (argc != 3)
//	{
//		printf("Usage : %s <IP> <port>\n", argv[0]);
//		exit(1);
//	}
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		ErrorHandling("WSAStartup() error!");
//
//	s = socket(PF_INET, SOCK_STREAM, 0);
//	if (s == INVALID_SOCKET)
//		ErrorHandling("socket() error!");
//
//	memset(&srvAddr, 0, sizeof(srvAddr));
//	srvAddr.sin_family = AF_INET;
//	inet_pton(AF_INET,argv[1],&srvAddr.sin_addr.s_addr);
//	srvAddr.sin_port = htons(atoi(argv[2]));
//
//	if (connect(s, (SOCKADDR*)& srvAddr, sizeof(srvAddr))==SOCKET_ERROR)
//		ErrorHandling("connect() error!");
//
//	if (fopen_s(&fp, "recive.dat", "wb") != 0)
//		ErrorHandling("fopen_s() error!");
//
//	while ((readCnt = recv(s, buff, sizeof(buff), 0)) != 0)
//		fwrite((void*)buff, 1, readCnt, fp);
//
//	puts("Received file data");
//	send(s, "Thank you", 10, 0);
//
//	fclose(fp);
//	closesocket(s);
//	WSACleanup();
//	return 0;
//
//}