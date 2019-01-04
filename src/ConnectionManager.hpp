#pragma once
#include <vector>
#include <memory>
#include "Definitions.hpp"
#include "TCPConnection.hpp"

namespace Server
{
	namespace Network
	{
		namespace Connection
		{
			namespace Manager
			{
				class ConnectionManager
				{
					std::vector<std::shared_ptr<TCPConnection>> _TCPConnections;
				public:
					ConnectionManager();
					//ConnectionManager(const ConnectionManager&) = delete;
					//ConnectionManager& operator=(const ConnectionManager&) = delete;
					void start(std::shared_ptr<TCPConnection> connection);
					void stop(std::shared_ptr<TCPConnection> connection);
					void stopAll();
				};
			}
		}
	}
}