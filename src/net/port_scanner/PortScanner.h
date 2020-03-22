#pragma once
#include <string>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include "../socket/Socket.h"
#include <memory>

class PortScanner
{
public:
	PortScanner();
	~PortScanner();
	std::vector<bool> scanAddress(std::wstring address);

private:
	std::shared_ptr<Socket> createSocket(std::wstring address, int port);
	static DWORD __stdcall portConnectionThreadProcedure(LPVOID context);

private:
	addrinfo hints;
};

