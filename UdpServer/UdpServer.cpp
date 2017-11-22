#include "UdpServer.h"
#include <iostream>


UdpServer::UdpServer()
{
	this->port = 80000;
	initializeWinSock();
}

UdpServer::UdpServer(const int& port)
{
	this->port = port;
	initializeWinSock();
}


UdpServer::~UdpServer()
{
}


void UdpServer::initializeWinSock()
{
	version = MAKEWORD(1, 0);

	// Handling errors
	if (WSAStartup(version, &WinSockData) != 0)
	{
		std::cerr << "\nCan't start WinSock." << std::endl;
		std::cerr << "Error #" << WSAGetLastError() << "." << std::endl;
		WSACleanup();
		return;
	}
	else
	{
		std::cout << "WinSock start succes." << std::endl;
	}
}

void UdpServer::createSocket()
{
	ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Handling errors
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cerr << "\nCan't create server socket." << std::endl;
		std::cerr << "Error #" << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}
	else
	{
		std::cout << "Server socket successfully created." << std::endl;
	}
}

void UdpServer::bindSocket()
{
	SocketAddressesServer.sin_family = AF_INET;
	SocketAddressesServer.sin_port = htons(port);
	//SocketAddressesServer.sin_addr.S_un.S_addr = INADDR_ANY;	// alternative: inet_pton
	SocketAddressesServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	clientAddressSize = sizeof(SocketAddressesClient);
	int bindError = bind(ServerSocket, (SOCKADDR*)&SocketAddressesServer, sizeof(SocketAddressesServer));
	if (bindError != 0)
	{
		std::cerr << "Can't bind socket." << std::endl;
		std::cerr << "Error #" << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
	}
	else
	{
		std::cout << "\nWaiting for client to send data..." << std::endl;
	}
}


void UdpServer::run()
{
	// Use private methods to properly configure server
	createSocket();
	bindSocket();

	unsigned buffer;
	int sleepTime;
	char messageToClient[32] = "CZESC";

	int receivingError = recvfrom(ServerSocket, (char*)&buffer, sizeof(int), 0, (SOCKADDR*)&SocketAddressesClient, &clientAddressSize);
	if (receivingError == SOCKET_ERROR)
	{
		std::cerr << "Can't receive data." << std::endl;
		std::cerr << "Error #" << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}
	else
	{
		sleepTime = (int)ntohl(buffer);
		std::cout << "CLIENT>" << sleepTime << std::endl;
		Sleep(sleepTime * 1000);
	}

	sendto(ServerSocket, (char*)&messageToClient, sizeof(messageToClient), 0, (sockaddr*)&SocketAddressesClient, sizeof(SocketAddressesServer));

	closesocket(ServerSocket);
	WSACleanup();

}