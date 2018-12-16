#pragma once
#include <string>
#include <vector>
#include <utility>
#include "Definitions.hpp"

namespace Server
{
	namespace Network
	{
		namespace Connection
		{
			namespace Manager
			{
				struct Request
				{
					std::string _data;
					std::string _method;
					std::string _uri;
					std::vector<Header> _headers;
					int _http_version_major;
					int _http_version_minor;
				};
			}
		}
	}
}
