#include <string>
#include <utility>
#include <signal.h>
#include "Definitions.hpp"
#include "ConnectionManager.hpp"
#include "TCPConnection.hpp"
#include "RequestHandler.hpp"
#include "Server.hpp"

using namespace Server::Network::Connection::Manager;

Server::Server::Server(const std::string& address = "0.0.0.0", 
					   const std::string& port = "80",
    				   const std::string& documentRoot = "./")
	: _ioContext(1),
	  _requestReceiver(_ioContext),
	  _signalReceiver(_ioContext),
	  _connectionManager(),
	  _port(port),
	  _address(address),
	  _requestHandler(documentRoot)
{
	#ifdef _DEBUG_
		printf("Server Constructor\n");
	#endif
	_signalReceiver.add(SIGINT);
	_signalReceiver.add(SIGTERM);
	#if defined(SIGQUIT)
		_signalReceiver.add(SIGQUIT);
	#endif // defined(SIGQUIT)

	awaitStop();

	TCPResolver resolver(_ioContext);
	TCPEndpoint endpoint =
		*resolver.resolve(_address, _port).begin();
	_requestReceiver.open(endpoint.protocol());
	_requestReceiver.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	_requestReceiver.bind(endpoint);
	_requestReceiver.listen();

	accept();
}

void Server::Server::start()
{
	#ifdef _DEBUG_
		printf("Server start()\n");
	#endif
	_ioContext.run();
}

void Server::Server::stop()
{
	#ifdef _DEBUG_
		printf("Server stop()\n");
	#endif
	_requestReceiver.close();
	_connectionManager.stopAll();
}

void Server::Server::accept()
{
	#ifdef _DEBUG_
		printf("Server Accept()\n");
	#endif
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
	#ifdef _DEBUG_
		printf("Server awaitStop()\n");
	#endif
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