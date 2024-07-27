#include "Connection.hpp"

Connection::Connection(int fd) : socket_fd(fd), reader(fd)
{
	std::cout << "new Connect, socket, fd = " << socket_fd << ", " << fd << std::endl;
	this->IsAfterResponseClose = 1;
	this->readingHeaderDone = 0;
	this->circle = 0;
	this->have_read = "";
}

Connection::~Connection()
{
	std::cout << "1 connect die" << std::endl;
}
// void Connection::reset()
// {
// 	this->IsAfterResponseClose = 1;
// 	this->readingHeaderDone = 0;
// 	this->circle = 0;

// 	// this->reader.done = 0;
// 	// this->reader.writer.done = 0;
// }
