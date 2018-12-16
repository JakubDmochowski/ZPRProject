#pragma once
#include "Definitions.hpp"

namespace Server
{
	namespace Network
	{
		namespace Connection
		{
			namespace Manager
			{
				class Connection
				{
				public:
					virtual void stop();
					virtual void start();
				};
			}
		}
	}
}