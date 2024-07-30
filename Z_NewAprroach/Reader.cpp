#include "Reader.hpp"
#include "Tomweb.hpp"
Reader::Reader() : writer()
{
	errNbr = 200;
	readingDone = 0;
	contentLength = 0;
	autoIndex = 1;
	method = "";
	URI = "";
	fdReadingFrom = -1;
	have_read = "";
	have_read_2 = "";
	openFile = 0;
	cnect_close = 0;
	post = 0;
	errFuncCall = 0;
	dir = NULL;
	host = "";
}

void	Reader::reset()
{
	
}

