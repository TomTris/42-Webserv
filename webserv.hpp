
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
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "Server/Server.hpp"

#define BUFFER_SIZE 1024
#define STANDARD404 "www/errors/error404.html"
#define STANDARD405 "www/errors/error405.html"

class Server;


struct location
{
    std::string url;
    std::vector<std::string> allowed;
    std::string root;
    bool autoindex;
    std::vector<std::string> indexes;
    std::vector<std::string> cgi_path;
    std::vector<std::string> cgi_ex;
    std::string temp;
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

std::string get_path_to_file(Server& s, std::string& path);
void set_bytes_to_zero(void *start, int len);
long long my_atoi(std::string numb);
int parse(std::string path, std::vector<server_t>& s);
std::string read_socket(int &fd);
int 	    method_find(int	&new_socket, std::string &method, std::string &path, std::string &request_content);
std::string read_file(int &fd);
location get_location(std::vector<location>& locations, std::string& path);


void	handle_unknown_request(int &new_socket, std::string &path);
void	handle_post_request(int	&new_socket, std::string &request_content, std::string &path);
void	handle_delete_request(int &new_socket, std::string &path);
void	handle_get_request(Server& s, int &new_socket, std::string &path);

#endif