#pragma once
#define WIN32_LEAN_AND_MEAN
#include "../Socket.h"
#include "sync/SocketSync.h"
#include <vector>
#include <memory>
#include <windows.h>

/*  Description:
		Used to create SocketSync objects, save their common state, save necessary resourses
		for thier parallel execution. Saves results of their execution as well.
*/

class SocketSyncBuilder
{
	friend class SocketSync;

public:
	SocketSyncBuilder(int maxCountPorts);
	~SocketSyncBuilder();
	std::unique_ptr<SocketSync> createSync(std::shared_ptr<Socket> socket);
	std::unique_ptr<SocketSync> createSync(std::shared_ptr<Socket> socket, size_t index);
	void setResult(bool result, size_t index);
	std::vector<bool> getResults();
	void start(std::unique_ptr<SocketSync> socketSync);

private:
	void initializePoolAndStuff(int maxCountPorts);
	void sync(bool result, size_t index);
	std::unique_ptr<SocketSync> commonCreateSync(std::shared_ptr<Socket> socket, size_t index);
	static void __stdcall workCallback(_Inout_ PTP_CALLBACK_INSTANCE Instance, _Inout_opt_ PVOID Context, _Inout_ PTP_WORK Work);

private:
	static constexpr int COUNT_SPINS = 4200;

	// std::numeric_limits<size_t>::max() does not work; use dirty trick: overflow
	static constexpr size_t INVALID_INDEX = -1; 

	size_t naturalIndex = 0;
	std::vector<bool> results;
	SYNCHRONIZATION_BARRIER barrier;
	CRITICAL_SECTION critical;
	PTP_POOL tpool;
	TP_CALLBACK_ENVIRON env;
	PTP_CLEANUP_GROUP cleanupGroup;
};

