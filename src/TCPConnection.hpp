#pragma once
#include <memory>
#include "Definitions.hpp"
#include "Connection.hpp"
#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"
#include "Request.hpp"
#include "Reply.hpp"
#include "Decoder.hpp"

namespace Server
{
	namespace Network
	{
		namespace Connection
		{
			namespace Manager
			{
				/// <summary>
				/// Represents a single TCP connection with a client
				/// </summary>
				class TCPConnection : public Connection, public std::enable_shared_from_this<TCPConnection>
				{
					TCPSocket _socket;
					ConnectionManager & _connectionManager;
					RequestHandler & _requestHandler;
					Decoder _decoder;

					Request _request;
					Reply _reply;

					std::array<char, 8192> _buffer;

					void read();
					void write();
				public:
					TCPConnection(const TCPConnection&) = delete;
					TCPConnection& operator=(const TCPConnection&) = delete;
					TCPConnection(TCPSocket socket, ConnectionManager & connectionManager, RequestHandler & requestHandler);

					virtual void start();
					virtual void stop();
				};
			}
		}
	}
}
