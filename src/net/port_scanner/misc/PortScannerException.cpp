#include "PortScannerException.h"

PortScannerException::PortScannerException()
	:
	Exception()
{
}

PortScannerException::PortScannerException(std::wstring message)
	:
	Exception(message)
{
}

PortScannerException& PortScannerException::operator<<(std::wstring message)
{
	Exception::operator<<(message);
	return *this;
}
