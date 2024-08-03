#ifndef TOMWEB_HPP
# define TOMWEB_HPP

#pragma once
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
#include <poll.h>
#include <filesystem> 
#include <ctime>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <algorithm>

#define ERROR200 "www/errors/200.html"
#define ERROR204 "www/errors/204.html"
#define ERROR400 "www/errors/400.html"
#define ERROR401 "www/errors/401.html"
#define ERROR402 "www/errors/402.html"
#define ERROR403 "www/errors/403.html"
#define ERROR404 "www/errors/404.html"
#define ERROR405 "www/errors/405.html"
#define ERROR408 "www/errors/408.html"
#define ERROR409 "www/errors/409.html"
#define ERROR411 "www/errors/411.html"
#define ERROR431 "www/errors/431.html"

#ifndef BUFFERSIZE
#define BUFFERSIZE 5000
#endif
#define TIME_OUT 5
#include "Connection.hpp"
struct location
{
    std::string URI;
    std::vector<std::string> allowed;
    std::string root;
    bool autoindex;
    std::vector<std::string> indexes;
    std::vector<std::string> cgi_path;
    std::vector<std::string> cgi_ex;
    int doesntExist;
    std::string returning;
};

struct server_t
{
    int host;
    int port;
    std::string servername;
    std::map<int, std::string> errorpages;
    int client_max_body_size;
    std::vector<location> locations;
};

class Server
{
    private:

    public:
        ~Server();
        Server(server_t& s);
        std::vector<std::vector<location> >             locations;
        std::vector<int>                                to_add_fds;
        std::vector<Connection>                         connections;
        std::vector<std::string>                        server_names;
        struct sockaddr_in                              address;
        std::map<int, std::string>                      errorPages;
        int                                             err;
        int                                             serverFd;
        int                                             body_size_max;
        int                                             port;
        int                                             host;
        std::string get_error_page(int numb);
        std::string return_default(int numb);
};

extern std::vector<struct pollfd>  fds;
//Parser
std::vector<std::string> get_data(std::string host, std::string method, std::string url, Server& serv);

//----------------------OB--------------------------//
void	load_config_n_socket_create(int ac, char **av, std::vector<Server> &servers);
int parse(std::string path, std::vector<server_t>& s);
bool checkIfFileExistsAndNotDirectory(std::string& path);
location get_location(std::vector<location>& locations, std::string& path);
std::vector<std::string> get_allowed(location& loc);
bool isAllowed(location& loc, std::string& method);
std::string get_path_of_standart_error(int errorNumb);
std::string get_error_page(Server& serv, int errorNumb);
std::string get_path_to_file(location& loc, std::string path);
bool isDirectory(const char *path);
long long my_atoi(std::string numb);
bool isNumber(std::string& number);
void handle_URI(std::string& URI);
bool checkIfFileExistsAndNotDirectory(std::string& path);
void set_bytes_to_zero(void *start, int len);

//----------------poll_revelant------------------//

void	add_servers_to_pool(std::vector<Server> &servers);

void	remove_from_poll(int fd_rm);
void	add_to_poll(int fd_add, int option);
void	change_option_poll(int fd, int option);

int 	connection_accept(Server &server);
int     check_fds(int fd);

//--------------server_level-----------//
void	server_level(std::vector<Server> &servers);
//------------Connection-level-------//
void	connection_level(std::vector<Server> &servers);

//----reader-----//
void	read_level(std::vector<Server> &servers);

//----writer----//
void	write_level(std::vector<Server> &servers);
//--Statuscode---//
std::string	get_header(int nbr, std::string str);
#endif