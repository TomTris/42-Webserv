#ifndef READER_HPP
#define READER_HPP

#pragma once
#include "Writer.hpp"
#include <string.h>
#include <iostream>
class Reader {
	public:
		Reader();
		Writer								writer;
		int                 				errNbr;
		int									openFile;
		int									readingDone; // reading part done

		int									contentLength;
		int									autoIndex;
		int									doesClientClose;

		int									connect_fd;
		std::string							method;
		std::string							URI;

		int									cnect_close;
		int									fdReadingFrom;

		std::string							have_read;
		std::string							have_read_2;
		std::string							to_write;
		void								reset();
};

#endif