#include <iostream>
#include <boost/asio.hpp>
#include "Server.hpp"

int main(int argc, char *argv[]) 
{
	try
	{   
		Server::Server server("0.0.0.0", "80", "./");
		server.start();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}
	return 0;
}