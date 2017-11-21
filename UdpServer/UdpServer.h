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
	sockaddr_in SocketAddressesServer;
	sockaddr_in SocketAddressesClient;

	void initializeWinSock();
	void createSocket();
	void bindSocket();

public:
	UdpServer();
	UdpServer(const int& port);
	~UdpServer();

	void run();
};

