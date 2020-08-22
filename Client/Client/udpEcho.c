//#include<stdio.h>
//#include<WinSock2.h>
//#include<WS2tcpip.h>
//void ErrorHandling(char* message)
//{
//	fputs(message, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}
//int main(int argc, char* argv[]) {
//	WSADATA wsaData;
//	SOCKET s;
//	char message[100];
//	int strLen;
//	SOCKADDR_IN srvAddr,fromAddr;
//	socklen_t adrSz;
//
//	if (argc != 3) {
//		printf("Usage : %s <IP> <port>\n", argv[0]);
//		exit(1);
//	}
//
//	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
//		ErrorHandling("WSAStartup() error!");
//	
//	s = socket(PF_INET, SOCK_DGRAM, 0);
//	if(s==INVALID_SOCKET)
//		ErrorHandling("socket() error");
//	memset(&srvAddr, 0, sizeof(srvAddr));
//	srvAddr.sin_family = AF_INET;
//	inet_pton(AF_INET, argv[1], &srvAddr.sin_addr.s_addr);
//	srvAddr.sin_port = htons(atoi(argv[2]));
//
//	while (1) {
//		fputs("Insert message(q to quit): ", stdout);
//		fgets(message, sizeof(message), stdin);
//		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
//			break;
//
//		sendto(s, message, strlen(message), 0, (SOCKADDR*)& srvAddr, sizeof(srvAddr));
//
//		adrSz = sizeof(fromAddr);
//		strLen = recvfrom(s, message, sizeof(message), 0, (SOCKADDR*)& srvAddr, sizeof(srvAddr));
//		
//		printf("Message from server: %s", message);
//	}
//	closesocket(s);
//	WSACleanup();
//}