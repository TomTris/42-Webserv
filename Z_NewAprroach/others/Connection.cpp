#include "Connection.hpp"
#include <unistd.h>
#include "../Tomweb.hpp"

Connection::Connection(int fd) : socket_fd(fd), reader()
{
	// std::cerr << "new Connect, socket, fd = " << socket_fd << ", " << fd << std::endl;
	IsAfterResponseClose = 0;
	readingHeaderDone = 0;
	have_read = "";
	time_out = time(NULL);
	
	reader.errNbr = 200;
	reader.openFile = 0;
	reader.readingDone = 0;

	reader.request_line_done = 0;
	reader.contentLength = 0;
	reader.autoIndex = 1;
	reader.method = "";
	reader.post = 0;
	reader.host = "";
	reader.URI = "";
	
	reader.cnect_close = 0;
	reader.fdReadingFrom = -1;
	
	reader.have_read_2 = "";
	reader.errFuncCall = 0;
	reader.dir = NULL;

	reader.time_out = time(NULL);

	reader.readCGI = 0;
	reader.childCreated = 0;
	reader.waitingDone = 0;
	reader.cookies = "";
	reader.pid = -1;
	reader.CGI_path = "";
	reader.query_string = "";
	reader.CGI_method = "";
	reader.content_type = "";
	reader.cgi_ex = "";
	reader.cgi_header_done = 0;
	reader.eof = 0;

	reader.writer.fdWritingTo = -1;
	reader.writer.writeString = "";
	reader.writer.writingDone = 0;
}

Connection::~Connection()
{
}

void Connection::reset()
{
	readingHeaderDone = 0;
	time_out = time(0);

	reader.errNbr = 200;
	reader.openFile = 0;
	reader.readingDone = 0;

	reader.request_line_done = 0;
	reader.contentLength = 0;
	reader.autoIndex = 1;
	
	reader.method = "";
	reader.post = 0;
	reader.host = "";
	reader.URI = "";

	change_option_poll(socket_fd, POLLIN);
	if (reader.fdReadingFrom != socket_fd && reader.fdReadingFrom != -1)
	{
		remove_from_poll(reader.fdReadingFrom);
		close(reader.fdReadingFrom);
	}
	reader.fdReadingFrom = -1;
	reader.errFuncCall = 0;
	reader.readCGI = 0;
	reader.childCreated = 0;
	reader.waitingDone = 0;
	reader.cookies = "";
	if (reader.pid != -1)
		kill(reader.pid, SIGTERM);
	reader.pid = -1;
	reader.CGI_path = "";
	reader.query_string = "";
	reader.CGI_method = "";
	reader.content_type = "";
	reader.cgi_ex = "";
	reader.cgi_header_done = 0;
	reader.eof = 0;

	
	if (reader.writer.fdWritingTo != socket_fd && reader.writer.fdWritingTo != -1)
	{
		remove_from_poll(reader.writer.fdWritingTo);
		close(reader.writer.fdWritingTo);
	}
	
	reader.writer.fdWritingTo = -1;
	reader.writer.writingDone = 0;
	time_out = time(NULL);
}

Reader::Reader() : writer()
{
}

Writer::Writer()
{
}
