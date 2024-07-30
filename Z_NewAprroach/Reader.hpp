#ifndef READER_HPP
#define READER_HPP

#pragma once
#include "Writer.hpp"
#include <string.h>
#include <iostream>
#include <dirent.h>
class Reader {
	public:
		Reader();
		Writer								writer;
		int                 				errNbr;
		int									openFile;
		int									readingDone; // reading part done

		int									contentLength;
		int									autoIndex;

		std::string							method;
		int									post;
		std::string							host;
		std::string							URI;

		int									cnect_close;
		int									fdReadingFrom;

		std::string							have_read;
		std::string							have_read_2;
		void								reset();
		time_t								time_out;
		DIR									*dir;
		int									errFuncCall;
};

#endif