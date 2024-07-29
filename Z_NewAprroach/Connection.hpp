#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#pragma once
#include "Reader.hpp"
#include <string.h>
#include <iostream>
class Connection {
	public:
        ~Connection();
		Connection(int fd);
        int							socket_fd;
		int									IsAfterResponseClose;
        int									readingHeaderDone;
        std::string							have_read;
		Reader								reader;
		time_t								time_out;
        void reset();
};

#endif
