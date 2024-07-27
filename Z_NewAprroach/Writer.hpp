#ifndef WRITER_HPP
#define WRITER_HPP


#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <algorithm>
#include <vector>
#include <map>

class Writer {
	public:
	
		Writer(int connect_fd, std::string &str);
		int					connect_fd;
		std::string			&writeString;
		int					fdWritingTo;
		int					done;
		void				reset();
};

#endif