#include "Connection.hpp"

Connection::Connection(int fd) : socket_fd(fd), reader()
{
	std::cerr << "new Connect, socket, fd = " << socket_fd << ", " << fd << std::endl;
	this->IsAfterResponseClose = 1;
	this->readingHeaderDone = 0;
	this->circle = 0;
	this->have_read = "";
}

Connection::~Connection()
{
}
