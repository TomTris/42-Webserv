#include "Reader.hpp"
#include "Tomweb.hpp"
Reader::Reader() : writer()
{
	errNbr = 200;
	contentLength = 0;
	autoIndex = 0;
	method = "";
	URI = "";
	doesClientClose = 0;
	fdReadingFrom = -1;
	have_read = "";
	to_write = "";
	openFile = 0;
	cnect_close = 0;
	have_read_2 = "";
}

void	Reader::reset()
{
	errNbr = 200;
	contentLength = 0;
	autoIndex = 0;
	method = "";
	URI = "";

	doesClientClose = 0;
	if (fdReadingFrom != connect_fd)
	{
		close(fdReadingFrom);
	}
	fdReadingFrom = -1;
	to_write = "" ;
	have_read = "";
	cnect_close = 0;
	have_read_2 = "";
}

