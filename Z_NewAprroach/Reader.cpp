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
	errFuncCall = 0;
	dir = NULL;
	time_out = clock();

	cookies = "";
}

