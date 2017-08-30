#include <WinSock2.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

void main()
{
	//�����׽���
	WSADATA wsaData;
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock");
		return;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(5099);
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	//�����׽���
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == sockClient)
	{
		printf("Socket() error:%d", WSAGetLastError());
		return;
	}

	//�������������������
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET)
	{
		printf("Connect failed:%d", WSAGetLastError());
		return;
	}
	else
	{
		//��������
		recv(sockClient, buff, sizeof(buff), 0);
		std::cout << "���շ������Ϣ" << std::endl;
		printf("%s\n", buff);
	}
	recv(sockClient, buff, sizeof(buff), 0);
	std::cout << buff << std::endl;

	char buffer[1024];
	//��������
	int i = 1;
	std::cout << "���ͷ������Ϣ" << std::endl;
	std::cin >> buffer;
	send(sockClient, buffer, sizeof(buffer), i);

	recv(sockClient, buff, sizeof(buff), 0);
	std::cout << buff << std::endl;

	//�ر��׽���
	closesocket(sockClient);
	WSACleanup();
}