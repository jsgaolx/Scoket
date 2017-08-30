#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

/*
recv ����
����ֵ<0������=0�����ӹرգ�>0���յ����ݴ�С��
*/
void main()
{
	WSADATA wsaData;
	int port = 5099;

	char buf[] = "Server: hello, I am a server.....";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock");
		return;
	}

	//�������ڼ������׽���
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port); //1024���ϵĶ˿ں�
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR){
		printf("Failed bind:%d\n", WSAGetLastError());
		return;
	}

	if (listen(sockSrv, 10) == SOCKET_ERROR){
		printf("Listen failed:%d", WSAGetLastError());
		return;
	}

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		//�ȴ��ͻ�������	
		SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
		if (sockConn == SOCKET_ERROR){
			printf("Accept failed:%d", WSAGetLastError());
			break;
		}
		printf_s("Accept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));

		//��������
		int iSend = send(sockConn, buf, sizeof(buf), 0);

		if (iSend == SOCKET_ERROR){
			printf("send failed");
			break;
		}

		std::cout << "�����뷢�͵���Ϣ" << std::endl;
		std::cin >> buf;
		iSend = send(sockConn, buf, sizeof(buf), 0);
		if (iSend == SOCKET_ERROR)
		{
			std::cout << "����ʧ��" << std::endl;
			break;
		}

		std::cout << "���յ���Ϣ" << std::endl;
		char recvBuf[100];
		memset(recvBuf, 0, sizeof(recvBuf));

		// 		//��������
		if (recv(sockConn, recvBuf, sizeof(recvBuf), 0) > 0)
		{
			printf("%s\n", recvBuf);
			send(sockConn, "�����ѽ���", sizeof(buf), 0);
		}
		std::cout << "���ӹر�" << std::endl;
		closesocket(sockConn);
	}

	closesocket(sockSrv);
	WSACleanup();
	system("pause");
}