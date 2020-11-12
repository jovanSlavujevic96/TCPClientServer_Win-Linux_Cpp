#include <iostream>

#include "Socket.h"
#include "ServerSocket.h"
#include "string.h"

#if defined(_MSC_VER)
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
#elif defined(__unix)

#endif

Socket::Socket(_SocketVal val, ServerSocket* server) :
	m_ClientSocket{ val },
	m_ServerSocket{ server }
{

}

Socket::~Socket()
{
#if defined(_MSC_VER)
	closesocket(m_ClientSocket);
#elif defined(__unix)
	close(m_ClientSocket);
#endif
}

Socket& Socket::operator<<(const char* data)
{
	// Echo message back to client
	send(m_ClientSocket, data, (int)strlen(data), 0);
	return *this;
}

Socket& Socket::operator>>(char* data)
{
	int bytesReceived = recv(m_ClientSocket, data, (int)m_ServerSocket->getBufferSize(), 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		std::cerr << "Error in recv(). Quitting\n";
		std::exit(-1); // TO DO
	}
	else if (bytesReceived == 0)
	{
		std::cout << "Client disconnected\n";
		std::exit(-1); // TO DO
	}
	return *this;
}