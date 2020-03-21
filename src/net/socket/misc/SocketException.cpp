#include "SocketException.h"

SocketException& SocketException::operator<<(std::wstring message)
{
	Exception::operator<<(message);
	return *this;
}
