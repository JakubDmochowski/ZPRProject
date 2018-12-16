#include "ConnectionManager.hpp"

using namespace Server::Network::Connection::Manager;

void ConnectionManager::start(std::shared_ptr<TCPConnection> c)
{
	_TCPConnections.push_back(c);
	c->start();
}

void ConnectionManager::stopAll()
{
	for (auto c : this->_TCPConnections)
		c->stop();
	_TCPConnections.clear();
}

void ConnectionManager::stop(std::shared_ptr<TCPConnection> c)
{
	_TCPConnections.erase(std::find(_TCPConnections.begin(), _TCPConnections.end(), c));
	c->stop();
}