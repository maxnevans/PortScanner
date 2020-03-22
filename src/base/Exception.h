#pragma once
#include <sstream>

class Exception
{
public:
	Exception() = default;
	Exception(std::wstring message);
	Exception(const Exception&);
	Exception& operator=(const Exception&);
	Exception(Exception&&) noexcept;
	Exception& operator=(Exception&&) noexcept;
	Exception& operator<<(std::wstring messagePart);
	virtual std::wstring what();

private:
	std::wstringstream messageStream;
};