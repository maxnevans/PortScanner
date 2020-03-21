#include "Socket.h"
#include "misc/SocketException.h"

Socket::Socket(addrinfo initData)
{
	sock = socket(initData.ai_family, initData.ai_socktype, initData.ai_protocol);

	if (sock == INVALID_SOCKET)
	{
		throw SocketException() << L"Failed to create socket!";
	}
}

Socket::Socket(Socket&& move)
	:
	sock(move.sock)
{
	move.sock = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket&& move)
{
	sock = move.sock;
	move.sock = INVALID_SOCKET;

	return *this;
}

Socket::~Socket()
{
	closesocket(sock);
}

Socket::operator SOCKET()
{
	return sock;
}

Socket::operator bool()
{
	return !*this;
}

bool Socket::operator!()
{
	return sock == INVALID_SOCKET;
}
