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
		std::cout << "Listening socket successfully created." << std::endl;
	}
}

void UdpServer::bindSocket()
{
	SocketAddressesServer.sin_family = AF_INET;
	SocketAddressesServer.sin_port = htons(port);
	SocketAddressesServer.sin_addr.S_un.S_addr = INADDR_ANY;	// alternative: inet_pton

	clientAddressSize = sizeof(SocketAddressesClient);
	bind(ServerSocket, (sockaddr*)&SocketAddressesServer, sizeof(SocketAddressesServer));

	std::cout << "\nWaiting for client to connect." << std::endl;
}


void UdpServer::run()
{
	// Use private methods to properly configure server
	createSocket();
	bindSocket();

	char hostRemoteAddress[NI_MAXHOST];
	char clientPortNumber[NI_MAXSERV];

	ZeroMemory(hostRemoteAddress, NI_MAXHOST);	// Just for safety
	ZeroMemory(clientPortNumber, NI_MAXSERV);

	// Prints client's remote address and port
	if (getnameinfo((sockaddr*)&SocketAddressesClient, sizeof(SocketAddressesClient), hostRemoteAddress, NI_MAXHOST, clientPortNumber, NI_MAXSERV, 0) == 0)
	{
		std::cout << "\n" << hostRemoteAddress << "\tport: " << clientPortNumber << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &SocketAddressesClient.sin_addr, hostRemoteAddress, NI_MAXHOST);
		std::cout << "\n" << hostRemoteAddress << "\tport: " << ntohs(SocketAddressesClient.sin_port) << std::endl;
	}

	char buffer[4096];
	ZeroMemory(&buffer, 4096);

	int receivingError = recvfrom(ServerSocket, buffer, sizeof(buffer), 0, (sockaddr*)&SocketAddressesServer, &clientAddressSize);
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
		std::cout << "CLIENT>" << buffer << std::endl;
	}

	sendto(ServerSocket, buffer, sizeof(buffer), 0, (sockaddr*)&SocketAddressesClient, sizeof(SocketAddressesServer));

	closesocket(ServerSocket);
	WSACleanup();

	//closesocket(ServerSocket);		// Closing listening socket

	//char buffer[4096];
	//ZeroMemory(&buffer, 4096);

	//// Wait for client to send data

	//char dataReceived = recv(ClientSocket, buffer, 4096, 0);
	//if (dataReceived == SOCKET_ERROR)
	//{
	//	std::cerr << "\nError while receiving data from client." << std::endl;
	//	std::cerr << "Error #" << WSAGetLastError() << std::endl;
	//	closesocket(ClientSocket);
	//	WSACleanup();
	//}
	//else
	//{
	//	std::cout << "\nCLIENT> " << std::string(buffer) << std::endl;
	//}

	//if (dataReceived == 0)
	//{
	//	std::cerr << "Client disconnected." << std::endl;
	//	closesocket(ClientSocket);
	//	WSACleanup();
	//}

	//// Send echo message
	//char tempByte;

	//for (int i = 0; i < dataReceived; i++)
	//{
	//	tempByte = buffer[i];
	//	send(ClientSocket, &tempByte, sizeof(tempByte), 0);
	//}

	//// Closing listening socket, cleanup
	//closesocket(ClientSocket);
	//WSACleanup();
}