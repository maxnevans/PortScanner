#include "PortScanner.h"
#include "misc/PortScannerException.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <comdef.h>
#include <string>
#include <vector>
#include "../socket/sync_builder/SocketSyncBuilder.h"

PortScanner::PortScanner()
{
	using namespace std::literals::string_literals;

	WSADATA wsaData = {};

	if (auto startupCode = WSAStartup(MAKEWORD(2, 2), &wsaData); startupCode != 0)
	{
		throw PortScannerException() << L"Failed to startup WSA...";
	}

	this->hints = {0};
	this->hints.ai_family = AF_UNSPEC;
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_protocol = IPPROTO_TCP;
}

PortScanner::~PortScanner()
{
	WSACleanup();
}

std::vector<bool> PortScanner::scanAddress(std::wstring address)
{
	return scanAddress(address, START_PORT, END_PORT);
}

std::vector<bool> PortScanner::scanAddress(std::wstring address, int startPort, int endPort)
{
	const int countPorts = endPort - startPort;
	auto builder = SocketSyncBuilder{ countPorts };

	for (size_t i = startPort; i < endPort; i++)
	{
		try {
			auto socket = createSocket(address, i);
			auto syncSocket = builder.createSync(std::move(socket), i);

			builder.start(std::move(syncSocket));
		}
		catch (PortScannerException& ) {
			builder.setResult(false, i);
			throw;
		}
	}

	return builder.getResults();
}

std::unique_ptr<Socket> PortScanner::createSocket(std::wstring address, int port)
{
	_bstr_t bAddr(address.c_str());
	const char* pAddress = bAddr;
	std::string portName = std::to_string(port);
	const char* pPort = portName.c_str();

	addrinfo* result = nullptr;

	if (auto status = getaddrinfo(pAddress, pPort, &this->hints, &result); status != 0)
	{
		throw PortScannerException() << L"Failed to resolve {" << address << L"}...";
	}

	return std::unique_ptr<Socket>{ new Socket{ *result } };
}
