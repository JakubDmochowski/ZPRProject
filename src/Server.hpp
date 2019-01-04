#pragma once
#include <string>
#include "Definitions.hpp"
#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"

using namespace Server::Network::Connection::Manager;

namespace Server
{
	class Server
	{
		ConnectionManager _connectionManager;
		RequestHandler _requestHandler;

		IOContext _ioContext;
		RequestReceiver _requestReceiver;

		SignalReceiver _signalReceiver;

		const std::string _port;
		const std::string _address;

		void accept();
		void awaitStop();
	public:
		Server(const std::string& address, const std::string& port, const std::string& documentRoot);
		void start();
		void stop();
	};
}