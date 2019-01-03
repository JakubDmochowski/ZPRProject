#pragma once
#include <string>
#include "Definitions.hpp"
#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"

namespace Server
{
	class Server
	{
		Network::Connection::Manager::ConnectionManager _connectionManager;
		Network::Connection::Manager::RequestHandler _requestHandler;

		Network::Connection::Manager::IOContext _ioContext;
		Network::Connection::Manager::RequestReceiver _requestReceiver;

		Network::Connection::Manager::SignalReceiver _signalReceiver;

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