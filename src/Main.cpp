#include <iostream>
#include "net/port_scanner/PortScanner.h"
#include "base/Exception.h"

#define START_PORT	0
#define END_PORT	1024

int main()
{
	std::wcout << L"Welcome to port scanner!\n"
		L"Enter port to start from: ";

	int startPort = START_PORT;
	std::wcin >> startPort;

	if (!std::wcin)
	{
		std::wcout << L"Wrong start port! Default will be used! (default = " << START_PORT << ")\n";
		std::wcin.clear();
	}

	std::wcout << L"Enter port to finish with (not included): ";

	int endPort = END_PORT;
	std::wcin >> endPort;

	if (!std::wcin)
	{
		std::wcout << L"Wrong finish port! Default will be used! (default = " << END_PORT << ")\n";
		std::wcin.clear();
	}

	std::wcout << L"Now wait about 20 seconds! We will try to connect to ports...\n";

	try {
		auto pc = PortScanner();

		auto portsOpen = pc.scanAddress(L"google.com", startPort, endPort);

		for (size_t i = 0; i < portsOpen.size(); i++)
		{
			std::wcout << L"Port " << i  << L" is " << (portsOpen[i] ? L"open" : L"closed") << '\n';
		}
	}
	catch (Exception& exception) {
		std::wcout << exception.what() << L'\n';
	}
	catch (...) {
		std::wcout << L"Oops!\n";
	}

	std::wcout << L"Program executed!\n";
	
	return 0;
}