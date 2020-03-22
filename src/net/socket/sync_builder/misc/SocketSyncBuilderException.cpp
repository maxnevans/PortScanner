#include "SocketSyncBuilderException.h"

SocketSyncBuilderException& SocketSyncBuilderException::operator<<(std::wstring message)
{
	Exception::operator<<(message);

	return *this;
}
