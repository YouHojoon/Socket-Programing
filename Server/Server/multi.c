#include<stdio.h>
#include<WinSock2.h>
#include<signal.h>


void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argv, char* argc) {
	
}