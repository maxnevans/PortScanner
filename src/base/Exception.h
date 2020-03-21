#pragma once
#include <sstream>

class Exception
{
public:
	Exception() = default;
	Exception(std::wstring message);
	Exception(const Exception&);
	Exception& operator=(const Exception&);
	Exception(Exception&&);
	Exception& operator=(Exception&&);
	Exception& operator<<(std::wstring messagePart);
	virtual std::wstring what();

private:
	std::wstringstream messageStream;
};