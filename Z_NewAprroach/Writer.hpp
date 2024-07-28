#ifndef WRITER_HPP
#define WRITER_HPP

#include <string.h>
#include <iostream>
class Writer {
	public:
	
		Writer();
		std::string			writeString;
		int					fdWritingTo;
};

#endif