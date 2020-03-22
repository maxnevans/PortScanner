#include "SocketSyncBuilder.h"
#include "misc/SocketSyncBuilderException.h"
#include <memory>

SocketSyncBuilder::SocketSyncBuilder(int maxCountPorts)
	:
	results(false, maxCountPorts)
{
	initializePoolAndStuff(maxCountPorts);

	// + 1 to wait in builder
	int barrierCount = maxCountPorts + 1; 

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
	CloseThreadpoolCleanupGroupMembers(cleanupGroup, TRUE, NULL);
	CloseThreadpoolCleanupGroup(cleanupGroup);
	CloseThreadpool(tpool);
}

std::unique_ptr<SocketSync> SocketSyncBuilder::createSync(std::shared_ptr<Socket> socket)
{	
	return commonCreateSync(socket, naturalIndex++);
}

std::unique_ptr<SocketSync> SocketSyncBuilder::createSync(std::shared_ptr<Socket> socket, size_t index)
{
	if (index < naturalIndex)
	{
		throw SocketSyncBuilderException() << L"Failed to create sync socket! Index {" << std::to_wstring(index) << L"} already exists!";
	}

	// +1 for the next
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

void SocketSyncBuilder::start(std::unique_ptr<SocketSync> socketSync)
{
	auto work = CreateThreadpoolWork(workCallback, socketSync.release(), &env);

	SubmitThreadpoolWork(work);
}

void SocketSyncBuilder::initializePoolAndStuff(int maxCountPorts)
{
	tpool = CreateThreadpool(NULL);

	if (tpool == NULL)
	{
		throw SocketSyncBuilderException() << L"Failed to create thread pool!";
	}

	SetThreadpoolThreadMaximum(tpool, maxCountPorts);
	SetThreadpoolThreadMinimum(tpool, maxCountPorts);

	InitializeThreadpoolEnvironment(&env);
	SetThreadpoolCallbackPool(&env, tpool);
	SetThreadpoolCallbackRunsLong(&env);

	cleanupGroup = CreateThreadpoolCleanupGroup();
	SetThreadpoolCallbackCleanupGroup(&env, cleanupGroup, NULL);
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

std::unique_ptr<SocketSync> SocketSyncBuilder::commonCreateSync(std::shared_ptr<Socket> socket, size_t index)
{
	return std::unique_ptr<SocketSync>{ new SocketSync(*this, socket, index) };
}

void __stdcall SocketSyncBuilder::workCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
	SocketSync* pSS = static_cast<SocketSync*>(Context);
	std::unique_ptr<SocketSync> ss = std::unique_ptr<SocketSync>(pSS);

	ss->execute();
}
