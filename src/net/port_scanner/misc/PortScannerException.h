#pragma once
#include "../../../base/Exception.h"
#include <string>

class PortScannerException :
	public Exception
{
public:
	PortScannerException();
	PortScannerException(std::wstring message);
	PortScannerException& operator<<(std::wstring message);
};

