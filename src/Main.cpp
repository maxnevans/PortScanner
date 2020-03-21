#include <iostream>
#include "net/port_scanner/PortScanner.h"
#include "base/Exception.h"

int main()
{
	try {
		auto pc = PortScanner();

		auto portsOpen = pc.scanAddress(L"google.com");

		for (size_t i = 0; i < portsOpen.size(); i++)
		{
			std::wcout << L"Port " << i  << L" is " << portsOpen[i] ? L"open" : L"closed\n";
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