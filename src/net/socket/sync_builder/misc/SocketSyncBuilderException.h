#pragma once
#include "../../../../base/Exception.h"

class SocketSyncBuilderException
	:
	public Exception
{
public:
	SocketSyncBuilderException& operator<<(std::wstring message);
};