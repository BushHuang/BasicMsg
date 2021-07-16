#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <bits/stdc++.h>
#include <windows.h>
#include <WinSock2.h>
#include <cstdio>
#include <pthread.h>

SOCKET sock;

void* rec(void* args)
{
	// ���ܷ�������Ϣ
	while(1)
	{
		char recvBuf[256] = {};
		int nlen = recv(sock, recvBuf, 1024, 0);
		if (nlen > 0)
			printf("%s\n", recvBuf);
	}
	return 0;
}

char msg[256];
std::string s;

int main()
{
	// ����Windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//----------------
	// ����һ��socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
		printf("�����׽��ִ���...\n");
	// ���ӷ����� connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(9527);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
		printf("���ӷ���������...\n");
	else
	{
		printf("����Socket�ɹ�...\n���������\n\n");
		pthread_t tid;
		ret=pthread_create(&tid,nullptr,rec,nullptr);
		if(ret!=0) printf("Pthread wrong!Num:%d\n",ret);
		else
		{
			do
			{
				if(strcmp(msg,"")!=0)
				{
					s="> ";
					s+=msg;
					send(sock,s.c_str(),sizeof(msg),0);
				}
				gets(msg);
			}
			while(strcmp(msg,"0")!=0);
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}