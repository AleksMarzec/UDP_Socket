#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


class UdpServer
{
private:
	WSAData WinSockData;
	WORD version;

	int port;
	int clientAddressSize;

	SOCKET ServerSocket;
	SOCKADDR_IN SocketAddressesServer;
	SOCKADDR_IN SocketAddressesClient;

	void initializeWinSock();
	void createSocket();
	void bindSocket();

public:
	UdpServer();
	UdpServer(const int& port);
	~UdpServer();

	void run();
};

