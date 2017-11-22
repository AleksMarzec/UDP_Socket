#include "UdpClient.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>


// Default constructor
UdpClient::UdpClient()
{
	this->port = 80000;
	this->IPadress = "127.0.0.1";
	initializeWinSock();
	createSocket();
}

// Constructor with user specified listening port
UdpClient::UdpClient(const int& port)
{
	this->port = port;
	this->IPadress = "127.0.0.1";
	initializeWinSock();
	createSocket();
}


UdpClient::~UdpClient()
{

}

void UdpClient::initializeWinSock()
{
	version = MAKEWORD(1, 0);

	// Handling errors
	if (WSAStartup(version, &WinSockData) != 0)
	{
		std::cerr << "Can't start WinSock." << std::endl;
		std::cerr << "Error #" << WSAGetLastError() << "." << std::endl;
		return;
	}
	else
	{
		std::cout << "WinSock start succes." << std::endl;
	}
}

// Creates client socket. Function required in constructor.
void UdpClient::createSocket()
{
	ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Handling errors and exceptions
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cerr << "Can't create a client socket." << std::endl;
		std::cerr << "Error #" << WSAGetLastError() << std::endl;
		return;
	}
	else
	{
		std::cout << "Client socket successfully created." << std::endl;
	}

	ReceiverAddresses.sin_family = AF_INET;
	ReceiverAddresses.sin_port = htons(this->port);
	//ReceiverAddresses.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, IPadress.c_str(), &ReceiverAddresses.sin_addr);

	//bind(ClientSocket, (sockaddr*)&ReceiverAddresses, sizeof(ReceiverAddresses));
}


// Closes client socket and cleans WS
void UdpClient::closeClean()
{
	closesocket(ClientSocket);
	WSACleanup();		// Deregisters itself from a Windows Sockets implementation and allows 
						//the implementation to free any resources allocated on behalf of the application or DLL
}

void UdpClient::run()
{
	char buffer;
	std::string userInput;
	int receiverAddressesSize = sizeof(ReceiverAddresses);

	std::cout << "\nSEND DATA" << std::endl;
	std::cout << ">> ";
	std::getline(std::cin, userInput);

	if (userInput.size() > 0)				// Checks if user has typed in something 
	{
		int sendDataError = sendto(ClientSocket, userInput.c_str(), sizeof(userInput.c_str()), 0, (SOCKADDR*)&ReceiverAddresses, sizeof(ReceiverAddresses));		// Sends user input data

		//if (sendDataError != SOCKET_ERROR)
		//{
		//	while (true)		// Receives server response
		//	{
		//		ZeroMemory(&buffer, 4096);		// Clears a block of memory
		//		int receivedBytes = recvfrom(ClientSocket, &buffer, sizeof(buffer), 0, (sockaddr*)&ReceiverAddresses, &receiverAddressesSize);

		//		if (receivedBytes > 0)
		//		{
		//			std::cout << "SERVER_RESPONSE> " << std::string(&buffer, 4096, receivedBytes) << std::endl;
		//		}
		//		if (receivedBytes == 0)
		//		{
		//			std::cout << "Server has ended the connection." << std::endl;
		//			break;
		//		}
		//	}
		//}
	}
	else
	{
		std::cerr << "Closing program. No input." << std::endl; // Exception
	}

	closeClean();
}
