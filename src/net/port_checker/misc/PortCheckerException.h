#pragma once
#include "../../../base/Exception.h"
#include <string>

class PortCheckerException :
	public Exception
{
public:
	PortCheckerException();
	PortCheckerException(std::wstring message);
	PortCheckerException& operator<<(std::wstring message);
};

