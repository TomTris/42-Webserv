#include "Reader.hpp"
#include "Tomweb.hpp"
Reader::Reader() : writer()
{
	errNbr = 200;
	openFile = 0;
	readingDone = 0;
	
	contentLength = 0;
	autoIndex = 1;

	method = "";
	post = 0;
	host = "";
	URI = "";

	cnect_close = 0;
	fdReadingFrom = -1;
	
	have_read = "";
	have_read_2 = "";
	time_out = time(NULL);
	dir = NULL;
	errFuncCall = 0;

	cookies = "";
	readCGI = 0;
	pid = -1;
	file_name1 = "";
}

