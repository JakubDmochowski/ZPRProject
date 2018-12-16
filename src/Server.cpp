#include <string>
#include <signal.h>
#include "Definitions.hpp"
#include "ConnectionManager.hpp"
#include "TCPConnection.hpp"
#include "RequestHandler.hpp"
#include "Server.hpp"

using namespace Server::Network::Connection::Manager;

Server::Server::Server()
	: _ioContext(1),
	  _requestReceiver(_ioContext),
	  _signalReceiver(_ioContext),
	  _connectionManager(),
	  _port("80"),
	  _address("128.0.0.1"),
	  _requestHandler("./")
{
	TCPResolver resolver(_ioContext);
	TCPEndpoint endpoint =
		*resolver.resolve(_address, _port).begin();
	_requestReceiver.open(endpoint.protocol());
	_requestReceiver.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	_requestReceiver.bind(endpoint);
	_requestReceiver.listen();
}

void Server::Server::start()
{
	_ioContext.run();
}

void Server::Server::stop()
{
	_requestReceiver.close();
	_connectionManager.stopAll();
}

void Server::Server::accept()
{
	_requestReceiver.async_accept(
		[this](ErrorCode ec, TCPSocket socket)
	{
		// Check whether the server was stopped by a signal before this
		// completion handler had a chance to run.
		if (!_requestReceiver.is_open()) return;
		if (!ec)
		{
			_connectionManager.start(std::make_shared<TCPConnection>(std::move(socket), _connectionManager, _requestHandler));
		}

		accept();
	});
}



void Server::Server::awaitStop()
{
	_signalReceiver.async_wait(
		[this](ErrorCode ec, int signo)
	{
		// The server is stopped by cancelling all outstanding asynchronous
		// operations. Once all operations have finished the io_context::run()
		// call will exit.
		_requestReceiver.close();
		_connectionManager.stopAll();
	});
}