#include "RequestHandler.hpp"
#include <fstream>
#include <sstream>
#include "Definitions.hpp"
#include "Request.hpp"
#include "Reply.hpp"
#include "MimeTypes.hpp"

using namespace Server::Network::Connection::Manager;

RequestHandler::RequestHandler(const std::string& documentRoot)
	: _documentRoot(documentRoot)
{
	#ifdef _DEBUG_
		printf("RequestHandler Constructor\n");
	#endif
}

void RequestHandler::handleRequest(const Request& req, Reply& rep)
{
	#ifdef _DEBUG_
		printf("RequestHandler handleRequest()\n");
	#endif
	// Decode url to path.
	std::string request_path;
	if (!urlDecode(req._uri, request_path))
	{
		rep = Reply::stockReply(Reply::statusType::bad_request);
		return;
	}

	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/'
		|| request_path.find("..") != std::string::npos)
	{
		rep = Reply::stockReply(Reply::statusType::bad_request);
		return;
	}

	// If path ends in slash (i.e. is a directory) then add "index.html".
	if (request_path[request_path.size() - 1] == '/')
	{
		request_path += "index.html";
	}

	// Determine the file extension.
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
	{
		extension = request_path.substr(last_dot_pos + 1);
	}

	// Open the file to send back.
	std::string full_path = _documentRoot + request_path;
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is)
	{
		rep = Reply::stockReply(Reply::statusType::not_found);
		return;
	}

	// Fill out the Reply to be sent to the client.
	rep._status = Reply::statusType::ok;
	char buf[512];
	while (is.read(buf, sizeof(buf)).gcount() > 0)
		rep._data.append(buf, is.gcount());
	rep._headers.resize(2);
	rep._headers[0].name = "Content-Length";
	rep._headers[0].value = std::to_string(rep._data.size());
	rep._headers[1].name = "Content-Type";
	rep._headers[1].value = mime_types::extensionToType(extension);
}

bool RequestHandler::urlDecode(const std::string& in, std::string& out)
{
	#ifdef _DEBUG_
		printf("RequestHandler urlDecode()\n");
	#endif
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i)
	{
		if (in[i] == '%')
		{
			if (i + 3 <= in.size())
			{
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value)
				{
					out += static_cast<char>(value);
					i += 2;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if (in[i] == '+')
		{
			out += ' ';
		}
		else
		{
			out += in[i];
		}
	}
	return true;
}