#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <bits/stdc++.h>
#include <windows.h>
#include <WinSock2.h>
#include <cstdio>
#include <pthread.h>
auto _cSock = INVALID_SOCKET;

void* Server_Setup(void* args)
{
	// ����Windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//----------------

	// --��Socket API��������TCP������
	// ����һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// �����ڽ��ܿͻ������ӵ�����˿�
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(9527);
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin)))
		printf("ERROR,�����ڽ��ܿͻ������ӵ�����˿�ʧ��...\n");
	else
		printf("�󶨶˿ڳɹ�...\n");

	// �����˿�
	if (SOCKET_ERROR == listen(_sock, 5))
		printf("��������˿�ʧ��...\n");
	else
		printf("��������˿ڳɹ�...\n");

	// ���տͻ�������
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);

	// ��ͻ��˷���һ������
	char msgBuf[] = "HaHa, you join the server!";
	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock)
		printf("���ܵ���Ч�ͻ���SOCKET...\n");
//	printf("�¿ͻ��˼��룺IP = %s:%d\n", inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);
	puts("�ͻ��˼��롣");
//	send(_cSock, msgBuf, sizeof(msgBuf), 0); //������Ϣ
	puts("һ��������ϡ�\n\n");
	while(1);
	closesocket(_sock);
	WSACleanup();
}

void* rec(void* args)
{
	char r[256];
	int nlen;
	while(1)
	{
		nlen=recv(_cSock,r,1024,0);
		if(nlen>0) printf("%s\n",r);
	}
}

char msg[256];
std::string s;

int main()
{
	pthread_t tid,tid2;
	int ret= pthread_create(&tid, nullptr,Server_Setup,nullptr);
	if(ret!=0) printf("Wrong:%d\n",ret);
	ret= pthread_create(&tid2, nullptr,rec,nullptr);
	if(ret!=0) printf("Wrong:%d\n",ret);
	else
	{
		do
		{
			if(strcmp(msg,"")!=0)
			{
				s="> ";
				s+=msg;
				send(_cSock,s.c_str(),sizeof(msg),0);
			}
			gets(msg);
		}
		while(strcmp(msg,"0")!=0);
	}
	return 0;
}