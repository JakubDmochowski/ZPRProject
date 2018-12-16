#pragma once
#include "Definitions.hpp"
#include "Request.hpp"
#include <string>

namespace Server
{
	namespace Network
	{
		namespace Connection
		{
			namespace Manager
			{
				class RequestHandler
				{
					std::string _documentRoot;
					static bool urlDecode(const std::string& input, std::string& output);

				public:
					RequestHandler(const RequestHandler&) = delete;
					RequestHandler& operator=(const RequestHandler&) = delete;
					explicit RequestHandler(const std::string& documentRoot);

					void handleRequest(const Request& request, Reply& reply);
				};
			}
		}
	}
}
