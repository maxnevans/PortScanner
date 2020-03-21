#pragma once
#include <string>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include "../socket/Socket.h"

class PortScanner
{
public:
	PortScanner();
	~PortScanner();
	std::vector<bool> scanAddress(std::wstring address);

private:
	Socket createConnection(std::wstring address, int port);

private:
	addrinfo hints;
};

