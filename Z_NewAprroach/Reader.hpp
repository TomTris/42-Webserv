#ifndef READER_HPP
#define READER_HPP

#pragma once
#include "Writer.hpp"
#include <string.h>
#include <iostream>
class Reader {
	public:
		Reader(int connect_fd);
		Writer								writer;
		int                 				errNbr;
		int									contentLength;
		int									autoIndex;
		int									doesClientClose;

		int									connect_fd;
		std::string							method;
		std::string							URI;

		int									fdReadingFrom;
		int									done;
		std::string							have_read;
		std::string							to_write;
		void								reset();
};

#endif