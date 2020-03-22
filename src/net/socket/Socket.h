#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

class Socket
{
public:
	Socket(addrinfo initData);
	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;
	Socket(Socket&&) noexcept;
	Socket& operator=(Socket&&) noexcept;
	~Socket();
	operator SOCKET();
	explicit operator bool();
	bool operator!();
	bool connect();

private:
	SOCKET sock;
	addrinfo bindedAddress;
};

