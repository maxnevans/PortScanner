#include "PortCheckerException.h"

PortCheckerException::PortCheckerException()
	:
	Exception()
{
}

PortCheckerException::PortCheckerException(std::wstring message)
	:
	Exception(message)
{
}

PortCheckerException& PortCheckerException::operator<<(std::wstring message)
{
	Exception::operator<<(message);
	return *this;
}
