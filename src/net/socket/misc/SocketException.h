#pragma once
#include "../../../base/Exception.h"

class SocketException :
	public Exception
{
public:
	SocketException& operator<<(std::wstring message);
};

