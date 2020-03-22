#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../Socket.h"
#include "sync/SocketSync.h"
#include <vector>
#include <memory>

class SocketSyncBuilder
{
	friend class SocketSync;

public:
	SocketSyncBuilder(int maxCountPorts);
	~SocketSyncBuilder();
	std::shared_ptr<SocketSync> createSync(std::shared_ptr<Socket> socket);
	std::shared_ptr<SocketSync> createSync(std::shared_ptr<Socket> socket, size_t index);
	void setResult(bool result, size_t index);
	std::vector<bool> getResults();

private:
	void sync(bool result, size_t index);
	std::shared_ptr<SocketSync> commonCreateSync(std::shared_ptr<Socket> socket, size_t index);

private:
	static constexpr int COUNT_SPINS = 4200;
	static constexpr size_t INVALID_INDEX = -1; // std::numeric_limits<size_t>::max() does not work; use dirty trick: overflow

	size_t naturalIndex = 0;
	std::vector<bool> results;
	SYNCHRONIZATION_BARRIER barrier;
	CRITICAL_SECTION critical;
};

