#include<stdio.h>
#include<WinSock2.h>
#include<stdlib.h>
#define BUF_SIZE 1024
void ErrorHandling(char* message);
void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
int recvBytes = 0;
char buf[BUF_SIZE];
int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hLisnSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;
	int recvAdrSz;

	WSABUF dataBuf;
	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	
	int flags = 0,idx;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hRecvSock = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&lisnAdr, 0, sizeof(lisnAdr));
	recvAdr.sin_family = AF_INET;
	recvAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	recvAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hRecvSock, (SOCKADDR*)& recvAdr, sizeof(recvAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hRecvSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	recvAdrSz = sizeof(lisnAdr);
	hLisnSock = accept(hRecvSock, (SOCKADDR*)& lisnAdr, &recvAdrSz);

	evObj = WSACreateEvent();
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;
	dataBuf.buf = buf;
	dataBuf.len = BUF_SIZE;

	if (WSARecv(hLisnSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, CompRoutine) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSA_IO_PENDING) {
			puts("Background data receive");
			//WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
			//WSAGetOverlappedResult(hLisnSock, &overlapped, &recvBytes, FALSE, NULL);
		}
	}
	idx = WSAWaitForMultipleEvents(1, &evObj, FALSE, WSA_INFINITE, TRUE);
	if (idx == WAIT_IO_COMPLETION)
		puts("Overlapped I/O Completed");
	else    // If error occurred!
		ErrorHandling("WSARecv() error");

	printf("Received message: %s \n", buf);
	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hLisnSock);
	WSACleanup();
	return 0;
}
void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
void CALLBACK CompRoutine(
	DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	if (dwError != 0)
	{
		ErrorHandling("CompRoutine error");
	}
	else
	{
		recvBytes = szRecvBytes;
		printf("Received message: %s \n", buf);
	}
}