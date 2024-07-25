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
#include <map>

struct location
{
    std::string url;
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

class Connection {
    private:

    public:
        Connection(int socket_fd_o);
        ~Connection();
        int                        IsAfterResponseClose;
        int                        socket_fd;
        int                        isReadingHeader;
        int                        isWriting; //write back to socket.
        int                        fdWritingTo;
        int                        needReadToTheEnd; // skip current request
        int                        doesClientClosed;
        int                        contentLength;

        std::string                host;

        std::string                contentType;
        std::string                contentDisposition;
        std::string                boundary;
        std::string                form_name;
        std::string                file_name;

        //after 10 second no request -> close
        std::string         method;
        std::string         URL;
        std::string         HTTP_version;

        //after 10 second not finished -> cancel
        int                 any_error;//err to return
        int                 circleTotal;//50: to handle a request. After finish 1 request-> = -1;
        int                 circleRead;// 20: waiting for new information too long -> time out
        int                 circleWrite;// = 100 -> close
        int                 infile;
        std::string         have_read;
};

class Server
{
    private:

    public:
        ~Server();
        Server(server_t& s);
        std::vector<Connection>       connections;
        std::vector<int>              to_add_fds;

        std::vector<location>         locations;
        struct sockaddr_in            address;
        std::map<int, std::string>    errorPages;
        int                           err;
        int                           serverFd;
        int                           body_size_max;
};

int parse(std::string path, std::vector<server_t>& s);
void	load_config_n_socket_create(int ac, char **av, std::vector<Server> &servers);

bool checkIfFileExistsAndNotDirectory(std::string& path);
location get_location(std::vector<location>& locations, std::string& path);
std::vector<std::string> get_allowed(location& loc);
bool isAllowed(location& loc, std::string& method);
std::string get_path_of_standart_error(int errorNumb);
std::string get_error_page(Server& serv, int errorNumb);
std::string get_path_to_file(location& loc, std::string path);
bool isDirectory(const char *path);

#endif