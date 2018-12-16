#pragma once
#include <string>
#include "Definitions.hpp"

namespace Server
{
	namespace Network
	{
		namespace Connection
		{
			namespace Manager
			{
				struct Reply
				{
					enum class statusType {
						ok = 200,
						created = 201,
						accepted = 202,
						no_content = 204,
						multiple_choices = 300,
						moved_permanently = 301,
						moved_temporarily = 302,
						not_modified = 304,
						bad_request = 400,
						unauthorized = 401,
						forbidden = 403,
						not_found = 404,
						internal_server_error = 500,
						not_implemented = 501,
						bad_gateway = 502,
						service_unavailable = 503
					};
					statusType _status;

					std::string _data;
					std::vector<Header> _headers;
					std::vector<ConstBuffer> to_buffers();

					/// Get a stock reply.
					static Reply stockReply(statusType status);
				};
			}
		}
	}
}
