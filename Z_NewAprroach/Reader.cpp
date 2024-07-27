#include "Reader.hpp"
#include "Tomweb.hpp"
Reader::Reader(int connect_fd) : connect_fd(connect_fd), writer(connect_fd)
{
	errNbr = 0;
	contentLength = 0;
	autoIndex = 0;

	method = "";
	URI = "";

	contentType = "";
	form_name = "";
	file_name = "";
	fdReadingFrom = -1;
	done = 1;
	have_read = "";
	to_write = "";
}

void	Reader::reset()
{
	errNbr = 0;
	contentLength = 0;
	autoIndex = 0;

	method = "";
	URI = "";

	contentType = "";
	form_name = "";
	file_name = "";
	if (fdReadingFrom != connect_fd)
	{
		close(fdReadingFrom);
	}
	fdReadingFrom = -1;
	done = 1;
	to_write = "" ;
	have_read = "";
}

