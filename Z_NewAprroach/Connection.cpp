#include "Connection.hpp"

Connection::Connection(int fd) : socket_fd(fd), reader()
{
	std::cerr << "new Connect, socket, fd = " << socket_fd << ", " << fd << std::endl;
	this->IsAfterResponseClose = 1;
	this->readingHeaderDone = 0;
	this->have_read = "";
	time_out = clock();
}

Connection::~Connection()
{
}

void Connection::reset()
{
	IsAfterResponseClose = 0;
	readingHeaderDone = 0;
	have_read = reader.have_read;
	reader.have_read = "";
	reader.have_read_2 = "";
	reader.cnect_close = 0;
	reader.contentLength = 0;
	reader.errNbr = 200;
	reader.fdReadingFrom = this->socket_fd;
	reader.method = "";
	reader.openFile = 0;
	reader.post = 0;
	reader.readingDone = 0;
	reader.URI = "";
	reader.writer.fdWritingTo = -1;
	reader.writer.writeString = "";
	time_out = clock();
}