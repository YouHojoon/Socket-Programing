//#include <stdio.h>
//#include <stdlib.h>
//#include <winsock2.h>
//void ErrorHandling(char* message);
//
//int main(int argc, char* argv[])
//{
//	WSADATA	wsaData;
//	SOCKET hServSock, hClntSock;
//	SOCKADDR_IN servAddr, clntAddr;
//	int strLen;
//	int szClntAddr;
//	char message[100];
//	
//	if (argc != 2)
//	{
//		printf("Usage : %s <port>\n", argv[0]);
//		exit(1);
//	}
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//윈도우 소켓 초기화
//		ErrorHandling("WSAStartup() error!");
//
//	hServSock = socket(PF_INET, SOCK_STREAM, 0);//소켓 초기화
//	if (hServSock == INVALID_SOCKET)
//		ErrorHandling("socket() error");
//
//	memset(&servAddr, 0, sizeof(servAddr));//소켓 주소 할당
//	servAddr.sin_family = AF_INET;
//	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//	servAddr.sin_port = htons(atoi(argv[1]));
//
//	if (bind(hServSock, (SOCKADDR*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR)
//		ErrorHandling("bind() error");
//
//	if (listen(hServSock, 5) == SOCKET_ERROR)
//		ErrorHandling("listen() error");
//
//	szClntAddr = sizeof(clntAddr);
//	for (int i = 0; i < 5; i++) {
//		hClntSock = accept(hServSock, (SOCKADDR*)& clntAddr, &szClntAddr);//클라이언트 연결 수락
//		if (hClntSock == INVALID_SOCKET)
//			ErrorHandling("accept() error");
//
//		recv(hClntSock, message, sizeof(message) - 1, 0);
//			int cnt = message[0];
//			printf("cnt:%d\n", cnt);
//
//			int *tmp=(int *)calloc(cnt,sizeof(int));
//
//			for (int i = 0; i < cnt; i++) {
//				tmp[i] = message[i + 1];
//				printf("operations %d :%d\n", i+1,tmp[i]);
//			}
//				
//			printf("operator %c :\n", message[cnt+1]);
//
//			int result = calculate(cnt, tmp, message[cnt + 1]);
//			send(hClntSock, (char *)&result, sizeof(result), 0);
//	
//		closesocket(hClntSock);
//	}
//	
//	closesocket(hServSock);
//	WSACleanup();
//	return 0;
//}
//int calculate(int opnum, int opnds[], char op)
//{
//	int result = opnds[0], i;
//
//	switch (op)
//	{
//	case '+':
//		for (i = 1; i < opnum; i++) result += opnds[i];
//		break;
//	case '-':
//		for (i = 1; i < opnum; i++) result -= opnds[i];
//		break;
//	case '*':
//		for (i = 1; i < opnum; i++) result *= opnds[i];
//		break;
//	}
//	return result;
//}
//void ErrorHandling(char* message)
//{
//	fputs(message, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}
