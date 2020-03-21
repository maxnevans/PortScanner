#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

class Socket
{
public:
	Socket(addrinfo initData);
	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;
	Socket(Socket&&);
	Socket& operator=(Socket&&);
	~Socket();
	operator SOCKET();
	explicit operator bool();
	bool operator!();

private:
	SOCKET sock;
};

