#include "SocketSync.h"

SocketSync::SocketSync(SocketSyncBuilder& builder, std::shared_ptr<Socket> socket, size_t index)
	:
	socket(socket),
	builder(builder),
	index(index)
{
}

void SocketSync::execute()
{
	bool success = socket->connect();

	builder.sync(success, index);
}
