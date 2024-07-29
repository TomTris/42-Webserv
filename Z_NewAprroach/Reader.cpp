#include "Reader.hpp"
#include "Tomweb.hpp"
Reader::Reader() : writer()
{
	errNbr = 200;
	contentLength = -1;
	autoIndex = 0;
	method = "";
	URI = "";
	doesClientClose = 0;
	fdReadingFrom = -1;
	have_read = "";
	have_read_2 = "";
	openFile = 0;
	cnect_close = 0;
	post = 0;
}

void	Reader::reset()
{
	
}

