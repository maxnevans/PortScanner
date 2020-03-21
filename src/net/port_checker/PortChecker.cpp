#include "PortChecker.h"
#include "misc/PortCheckerException.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <comdef.h>
#include <string>
#include <vector>


PortChecker::PortChecker()
{
	using namespace std::literals::string_literals;

	WSADATA wsaData = {};

	if (auto startupCode = WSAStartup(MAKEWORD(2, 2), &wsaData); startupCode != 0)
	{
		throw PortCheckerException() << L"Failed to startup WSA...";
	}

	this->hints = {0};
	this->hints.ai_family = AF_UNSPEC;
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_protocol = IPPROTO_TCP;
}

PortChecker::~PortChecker()
{
	WSACleanup();
}

std::vector<bool> PortChecker::scanAddress(std::wstring address)
{
	std::vector<bool> success(1024, false);
	for (int i = 0; i < 1024; i++)
	{
		try {
			Socket socket = createConnection(address, i);
			success[i] = socket;
		}
		catch (PortCheckerException& ) {
			success[i] = false;
		}
	}

	return success;
}

Socket PortChecker::createConnection(std::wstring address, int port)
{
	_bstr_t bAddr(address.c_str());
	const char* pAddress = bAddr;
	std::string portName = std::to_string(port);
	const char* pPort = portName.c_str();

	addrinfo* result = nullptr;

	if (auto status = getaddrinfo(pAddress, pPort, &this->hints, &result); status != 0)
	{
		throw PortCheckerException() << L"Failed to resolve {" << address << L"}...";
	}

	Socket socket{*result};

	if (auto socketConnectStatus = connect(socket, result->ai_addr, result->ai_addrlen); socketConnectStatus == SOCKET_ERROR)
	{
		throw PortCheckerException() << L"Failed to connect to {" << address << L"}!";
	}

	return socket;
}
