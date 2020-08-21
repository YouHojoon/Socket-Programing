#include<stdio.h>
#include<winsock2.h>
#include<WS2tcpip.h>
void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}
int main(int argc, char* argv[]) {
	
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN srvAddr;
	char buffer[100];

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0)
		ErrorHandling("WSAStartup() error!");
	
	s = socket(PF_INET, SOCK_STREAM, 0);
	if(s==INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&srvAddr, 0, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &srvAddr.sin_addr.s_addr);
	srvAddr.sin_port = htons(atoi(argv[2]));

	if (connect(s, (SOCKADDR*)& srvAddr, sizeof(srvAddr))==SOCKET_ERROR)
		ErrorHandling("connet() error");
	int cnt;

		fputs("Operation count: ", stdout);
		scanf("%d", &cnt);
		buffer[0] = cnt;


		for (int i = 0; i < cnt; i++) {
			printf("Operation %d: ", i+1);
			scanf("%d", (int *)&buffer[i+1]);	
		}
		fgetc(stdin);
		fputs("Operator : ", stdout);
		scanf("%c", &buffer[cnt+1]);

		send(s, buffer, cnt+2, 0);

		if (recv(s, &cnt, 1, 0) == -1)
			ErrorHandling("read() error");

		printf("result : %d\n", cnt);

	closesocket(s);
	WSACleanup();
	return 0;
}
