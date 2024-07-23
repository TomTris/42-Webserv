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

class   Request {
    private:

/* any error -> set any_error = true;
open error with infile.
empty have read, read it into have_read, when it gets write_fds, it will write into inside
after read -> call to del.
1. Method, path, http version
2.1 GET: - path -> return.
2.2 POST - path content-length-left = ? 
open(PATH);
read -> have_read
write_fds -> write
If not enough -> infile = iopen (bad request) -> delete file.
2.3 DELETE - path -> done -> ret
*/
//GET request: should not have content-length or must = 0;
//POST request: must contain content-length
    public:
        Request(int *isError);
        int                 *isError;
        int                 isFinished;

        std::string         method;
        std::string         URL;
        std::string         HTTP_version;
        //after 10 second not finished -> cancel
        int                 any_error;//err to return
        int                 circle_read;// = 100 -> close
        int                 circle_write;// = 100 -> close
        int                 content_length_left;
        int                 infile;
        std::string         have_read;
        bool                response_done;
};

class Connection {
    private:

    public:
        Connection(int socket_fd_o);
        ~Connection();
        int                        isError;
        int                 socket_fd;
        std::vector<Request>    requests;
        //after 10 second no request -> close
        struct timeval      tv;
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

#endif