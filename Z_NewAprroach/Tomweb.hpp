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

class Connection;
class Server
{
    private:

    public:
        ~Server();
        Server(server_t& s);
        std::vector<std::vector<location> >             locations;
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

#include "Connection.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

//----------------------OB--------------------------//
void	load_config_n_socket_create(int ac, char **av, std::vector<Server> &servers, int &max_fd);
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
//----------------------OB--------------------------//

#endif