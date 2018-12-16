#pragma once
#include <boost/asio.hpp>
#include <string>
#include <utility>
#include "Header.hpp"

namespace Server
{
	namespace Network
	{
		namespace Connection
		{
			namespace Manager
			{
				using Header = header;

				using TCPEndpoint = boost::asio::ip::tcp::endpoint;
				using TCPResolver = boost::asio::ip::tcp::resolver;
				using TCPSocket = boost::asio::ip::tcp::socket;
				using IOContext = boost::asio::io_context;
				using RequestReceiver = boost::asio::ip::tcp::acceptor;
				using ErrorCode = boost::system::error_code;
				using ConstBuffer = boost::asio::const_buffer;
				using SignalReceiver = boost::asio::signal_set;

				class Connection;
				class ConnectionManager;
				class Decoder;
				class Encoder;
				class RequestHandler;
				class RequestSender;
				class TCPConnection;
				struct Reply;
				struct Request;
			}
		}
	}
}