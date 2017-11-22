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
	char buffer[32];
	std::string userInput;
	int L;
	int receiverAddressesSize = sizeof(ReceiverAddresses);

	std::cout << "\nSEND AMOUNT OF SECONDS (INT)" << std::endl;
	std::cout << ">> ";
	std::cin >> L;

	if (!std::cin.fail())				// Checks if user has typed in something 
	{
		//int sendDataError = sendto(ClientSocket, userInput.c_str(), sizeof(userInput.c_str()), 0, (SOCKADDR*)&ReceiverAddresses, sizeof(ReceiverAddresses));		// Sends user input data
		unsigned sleepTime = htonl((unsigned)L);
		int sendDataError = sendto(ClientSocket, (char*)&sleepTime, sizeof(L), 0, (SOCKADDR*)&ReceiverAddresses, sizeof(ReceiverAddresses));
		if (sendDataError != SOCKET_ERROR)
		{
			ZeroMemory(&buffer, 32);		// Clears a block of memory
			
			while (true)
			{
				int receivedBytes = recvfrom(ClientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&ReceiverAddresses, &receiverAddressesSize);
				if (receivedBytes > 0)
				{
					//std::cout << "SERVER_RESPONSE> " << std::string(&buffer, 4096, receivedBytes) << std::endl;
					std::cout << "SERVER_RESPONSE> " << buffer << std::endl;

				}
			}
		}
		else
		{
			std::cerr << "Error while sending data." << std::endl;
			std::cerr << "Error #" << WSAGetLastError() << std::endl;
		}
	}
	else
	{
		std::cerr << "Closing program. Bad data type or no input." << std::endl; // Exception
	}
	closeClean();
}
