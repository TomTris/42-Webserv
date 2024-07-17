
#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <sys/event.h>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

#include "Server/Server.hpp"


struct location
{
    std::string url;
    std::vector<std::string> allowed;
    std::string root;
    bool autoindex;
    std::vector<std::string> indexes;
    std::vector<std::string> cgi_path;
    std::vector<std::string> cgi_ex;
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

void set_bytes_to_zero(void *start, int len);
long long my_atoi(std::string numb);
int parse(std::string path, std::vector<server_t>& s);

#endif