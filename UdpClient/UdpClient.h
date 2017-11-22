#pragma once
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

class UdpClient
{
private:
	WSAData WinSockData;
	WORD version;

	int port;

	std::string IPadress;
	SOCKET ClientSocket;
	SOCKADDR_IN ReceiverAddresses;
	sockaddr_in SenderAddresses;

	void initializeWinSock();
	void createSocket();
	void closeClean();
public:
	UdpClient();
	UdpClient(const int& port);
	~UdpClient();

	void run();
};

