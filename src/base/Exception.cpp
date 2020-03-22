#include "Exception.h"
#include <memory>

Exception::Exception(std::wstring message)
{
	this->messageStream << message;
}

Exception::Exception(const Exception& copy)
	:
	messageStream(copy.messageStream.str())
{
}

Exception& Exception::operator=(const Exception& copy)
{
	this->messageStream << copy.messageStream.str();
	return *this;
}

Exception::Exception(Exception&& move) noexcept
{
	this->messageStream << std::move(move.messageStream.str());
}

Exception& Exception::operator=(Exception&& move) noexcept
{
	this->messageStream << std::move(move.messageStream.str());
	return *this;
}

Exception& Exception::operator<<(std::wstring messagePart)
{
	this->messageStream << messagePart;
	return *this;
}

std::wstring Exception::what()
{
	return this->messageStream.str();
}
