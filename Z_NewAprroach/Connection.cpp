#include "Connection.hpp"

Connection::Connection(Server &server, int fd) : server(server), socket_fd(fd), reader(fd)
{
	this->IsAfterResponseClose = 1;
	this->readingHeaderDone = 0;
	this->circle = 0;
	this->have_read = "";
}

void Connection::reset()
{
	this->IsAfterResponseClose = 1;
	this->readingHeaderDone = 0;
	this->circle = 0;

	// this->reader.done = 0;
	// this->reader.writer.done = 0;
}
