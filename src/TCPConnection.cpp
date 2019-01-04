#include "TCPConnection.hpp"
#include <memory>
#include "Decoder.hpp"

using namespace Server::Network::Connection::Manager;

TCPConnection::TCPConnection(TCPSocket socket, ConnectionManager & connectionManager, RequestHandler & requestHandler)
	: _socket(std::move(socket)), _connectionManager(connectionManager), _requestHandler(requestHandler)
{
	#ifdef _DEBUG_
		printf("TCPConnection Constructor\n");
	#endif
}

void TCPConnection::start()
{
	#ifdef _DEBUG_
		printf("TCPConnection Start()\n");
	#endif
	read();
}

void TCPConnection::stop()
{
	#ifdef _DEBUG_
		printf("TCPConnection Stop()\n");
	#endif
	_socket.close();
}

void TCPConnection::read()
{
	#ifdef _DEBUG_
		printf("TCPConnection Read()\n");
	#endif
	auto self(shared_from_this());
	_socket.async_read_some(boost::asio::buffer(_buffer),
		[this, self](boost::system::error_code ec, std::size_t bytes_transferred)
	{
		if (!ec)
		{
			Decoder::result_type result;
			std::tie(result, std::ignore) = _decoder.parse(
				_request, _buffer.data(), _buffer.data() + bytes_transferred);

			if (result == Decoder::good)
			{
				_requestHandler.handleRequest(_request, _reply);
				write();
			}
			else if (result == Decoder::bad)
			{
				_reply = Reply::stockReply(Reply::statusType::bad_request);
				write();
			}
			else
			{
				read();
			}
		}
		else if (ec != boost::asio::error::operation_aborted)
		{
			_connectionManager.stop(shared_from_this());
		}
	});
}

void TCPConnection::write()
{
	#ifdef _DEBUG_
		printf("TCPConnection Write()\n");
	#endif
	auto self(shared_from_this());
	boost::asio::async_write(_socket, _reply.to_buffers(),
		[this, self](boost::system::error_code ec, std::size_t)
	{
		if (!ec)
		{
			// Initiate graceful connection closure.
			boost::system::error_code ignored_ec;
			_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		}

		if (ec != boost::asio::error::operation_aborted)
		{
			_connectionManager.stop(shared_from_this());
		}
	});
}


