#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

/*
recv 函数
返回值<0：出错，=0：连接关闭，>0接收到数据大小。
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

	//创建用于监听的套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port); //1024以上的端口号
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
		//等待客户请求到来	
		SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
		if (sockConn == SOCKET_ERROR){
			printf("Accept failed:%d", WSAGetLastError());
			break;
		}
		printf_s("Accept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));

		//发送数据
		int iSend = send(sockConn, buf, sizeof(buf), 0);

		if (iSend == SOCKET_ERROR){
			printf("send failed");
			break;
		}

		std::cout << "请输入发送的消息" << std::endl;
		std::cin >> buf;
		iSend = send(sockConn, buf, sizeof(buf), 0);
		if (iSend == SOCKET_ERROR)
		{
			std::cout << "发送失败" << std::endl;
			break;
		}

		std::cout << "接收的消息" << std::endl;
		char recvBuf[100];
		memset(recvBuf, 0, sizeof(recvBuf));

		// 		//接收数据
		if (recv(sockConn, recvBuf, sizeof(recvBuf), 0) > 0)
		{
			printf("%s\n", recvBuf);
			send(sockConn, "数据已接收", sizeof(buf), 0);
		}
		std::cout << "链接关闭" << std::endl;
		closesocket(sockConn);
	}

	closesocket(sockSrv);
	WSACleanup();
	system("pause");
}