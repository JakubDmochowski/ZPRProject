#include "ConnectionManager.hpp"

using namespace Server::Network::Connection::Manager;

ConnectionManager::ConnectionManager()
{
	#ifdef _DEBUG_
		printf("ConnectionManager Constructor\n");
	#endif
}

void ConnectionManager::start(std::shared_ptr<TCPConnection> c)
{
	#ifdef _DEBUG_
		printf("ConnectionManager start()\n");
	#endif
	_TCPConnections.push_back(c);
	c->start();
}

void ConnectionManager::stopAll()
{
	#ifdef _DEBUG_
		printf("ConnectionManager stopAll()\n");
	#endif
	for (auto c : this->_TCPConnections)
		c->stop();
	_TCPConnections.clear();
}

void ConnectionManager::stop(std::shared_ptr<TCPConnection> c)
{
	#ifdef _DEBUG_
		printf("ConnectionManager stop()\n");
	#endif
	_TCPConnections.erase(std::find(_TCPConnections.begin(), _TCPConnections.end(), c));
	c->stop();
}