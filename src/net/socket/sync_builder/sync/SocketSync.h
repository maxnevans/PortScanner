#pragma once
#include "../../Socket.h"
#include "../SocketSyncBuilder.h"
#include <memory>

class SocketSyncBuilder;

class SocketSync
{
public:
	SocketSync(SocketSyncBuilder& builder, std::shared_ptr<Socket> socket, size_t index);
	void execute();

private:
	SocketSyncBuilder& builder;
	std::shared_ptr<Socket> socket;
	size_t index;
};