#ifndef WRITER_HPP
#define WRITER_HPP

#include <string.h>
#include <iostream>
class Writer {
	public:
	
		Writer(int connect_fd);
		int					connect_fd;
		std::string			writeString;
		int					fdWritingTo;
		int					done;
		void				reset();
};

#endif