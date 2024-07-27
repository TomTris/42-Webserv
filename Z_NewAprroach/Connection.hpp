#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#pragma once
#include "Tomweb.hpp"
#include "Reader.hpp"

class Connection {
	public:
		Connection(Server &server, int fd);
		Server								&server;
        int									socket_fd;

		int									IsAfterResponseClose;
        int									readingHeaderDone;
        int									circle; //waiting for request
        std::string							have_read;
        void								reset();
		Reader								reader;
};

#endif
