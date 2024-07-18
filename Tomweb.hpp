#ifndef TOMWEB_HPP
# define TOMWEB_HPP

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

typedef struct s_server_config {
    int         port;
    std::string root;
    int         listen_max;
} t_server_config;

typedef struct s_server_data_saver {
    s_server_config     config;
    int                 listen_max;
    struct sockaddr_in  server_addr;
    int                 server_fd;
}   t_server_data_saver;


#endif