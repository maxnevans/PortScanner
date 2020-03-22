#include "SocketSyncBuilder.h"
#include "misc/SocketSyncBuilderException.h"
#include <memory>

SocketSyncBuilder::SocketSyncBuilder(int maxCountPorts)
	:
	results(false, maxCountPorts)
{
	int barrierCount = maxCountPorts + 1; //to wait in builder

	if (InitializeSynchronizationBarrier(&barrier, barrierCount, -1) == FALSE)
	{
		throw SocketSyncBuilderException() << L"Failed to initialize barrier!";
	}

	if (InitializeCriticalSectionAndSpinCount(&critical, SocketSyncBuilder::COUNT_SPINS) == FALSE)
	{
		throw SocketSyncBuilderException() << L"Failed to initialize critical section!";
	}
}

SocketSyncBuilder::~SocketSyncBuilder()
{
	DeleteSynchronizationBarrier(&barrier);
	DeleteCriticalSection(&critical);
}

std::shared_ptr<SocketSync> SocketSyncBuilder::createSync(std::shared_ptr<Socket> socket)
{	
	return commonCreateSync(socket, naturalIndex++);
}

std::shared_ptr<SocketSync> SocketSyncBuilder::createSync(std::shared_ptr<Socket> socket, size_t index)
{
	if (index < naturalIndex)
	{
		throw SocketSyncBuilderException() << L"Failed to create sync socket! Index {" << std::to_wstring(index) << L"} already exists!";
	}

	naturalIndex = index + 1;

	return commonCreateSync(socket, index);
}

void SocketSyncBuilder::setResult(bool result, size_t index)
{
	EnterCriticalSection(&critical);
	results[index] = result;
	LeaveCriticalSection(&critical);
}

std::vector<bool> SocketSyncBuilder::getResults()
{
	EnterSynchronizationBarrier(&barrier, NULL);

	EnterCriticalSection(&critical);
	auto resultsCopy = results;
	LeaveCriticalSection(&critical);

	return resultsCopy;
}

void SocketSyncBuilder::sync(bool result, size_t index)
{
	EnterCriticalSection(&critical);

	if (results.size() < naturalIndex)
	{
		results.resize(naturalIndex, false);
	}

	results[index] = result;

	LeaveCriticalSection(&critical);

	EnterSynchronizationBarrier(&barrier, NULL);
}

std::shared_ptr<SocketSync> SocketSyncBuilder::commonCreateSync(std::shared_ptr<Socket> socket, size_t index)
{
	return std::shared_ptr<SocketSync>{ new SocketSync(std::shared_ptr<SocketSyncBuilder>(this), socket, index) };
}
