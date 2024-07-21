#ifndef TOMWEB_HPP
# define TOMWEB_HPP

#define BUFFER_SIZE 1024
#define PORT 8081
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

typedef struct s_server_config {
    int         port;
    std::string root;
    int         listen_max;
} t_server_config;

typedef struct s_server_data {
    s_server_config     config;
    struct sockaddr_in  server_addr;
    int                 server_fd;
}   t_server_data;

std::string read_socket(int &fd);
int 	    method_find(int	&new_socket, std::string &method, std::string &path, std::string &request_content);
void    	load_config(int ac, char **av, t_server_config &config);
int	        socket_create(t_server_config &config, struct sockaddr_in &server_addr);
std::string read_file(int &fd);

void	set_init(fd_set &read_fds, t_server_data &server_data, int &max_fd, std::vector<int> &client_socket);


void	handle_unknown_request(int &new_socket, std::string &path);
void	handle_post_request(int	&new_socket, std::string &request_content, std::string &path);
void	handle_delete_request(int &new_socket, std::string &path);
void	handle_get_request(int &new_socket, std::string &path);

#endif