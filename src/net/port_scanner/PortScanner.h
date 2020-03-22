#pragma once
#include <string>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include "../socket/Socket.h"
#include <memory>

/*  Description:
		Main application class. Use to scan for ports.
*/

class PortScanner
{
public:
	PortScanner();
	~PortScanner();
	std::vector<bool> scanAddress(std::wstring address);
	std::vector<bool> scanAddress(std::wstring address, int startPort, int endPort);

private:
	std::unique_ptr<Socket> createSocket(std::wstring address, int port);

private:
	static constexpr size_t START_PORT = 0;
	static constexpr size_t END_PORT = 1024;
	static constexpr size_t COUNT_PORTS = END_PORT - START_PORT;

	addrinfo hints;
};

