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

class Server;

class Connection {
    private:

    public:
        Connection(int socket_fd);
        ~Connection();
        int                                 IsAfterResponseClose;
        int                                 socket_fd;
        int                                 isReadingHeader;
        int                                 isWriting; //write back to socket.
        int                                 fdWritingTo;
        int                                 doesClientClosed;
        int                                 contentLength;

        std::string                         host;
        std::string                         contentType;
        std::string                         contentDisposition;
        std::string                         boundary;
        std::string                         form_name;
        std::string                         file_name;

        std::string         method;
        std::string         URI;
        std::string         HTTP_version;

        int                 errNbr;
        int                 circleTotal;
        int                 circleRead;
        int                 circleWrite;

        std::string         have_read;
    
        void    reset();
};

class Server
{
    private:

    public:
        ~Server();
        Server(server_t& s);
        std::vector<Connection>                 connections;
        std::vector<int>                        to_add_fds;
        std::vector<location>                   locations;
        struct sockaddr_in                      address;
        std::map<int, std::string>              errorPages;
        int                                     err;
        int                                     serverFd;
        int                                     body_size_max;

        void accept();
};

int parse(std::string path, std::vector<server_t>& s);
bool checkIfFileExistsAndNotDirectory(std::string& path);
location get_location(std::vector<location>& locations, std::string& path);
std::vector<std::string> get_allowed(location& loc);
bool isAllowed(location& loc, std::string& method);
std::string get_path_of_standart_error(int errorNumb);
std::string get_error_page(Server& serv, int errorNumb);
std::string get_path_to_file(location& loc, std::string path);
bool isDirectory(const char *path);

int	open_file(Connection &current_connection);
int	body_handle_post(Connection &current_connection);
int	connection_delete(int &fd_to_del, std::vector<Server> &servers);
int	connections_delete(std::vector<int> &to_del_fds, std::vector<Server> &servers);
void	connection_accept(Server &server);
void connections_add(std::vector<Server> &servers);
int	extract_IsAfterResponseClose(Connection &current_connection, std::string &header_o);
int	extract_contentLength(Connection &current_connection, std::string &header_o);
int	extract_contentType(Connection &current_connection, std::string &header_o);
int	extract_host(Connection &current_connection, std::string &header_o);
int	extract_boundary(Connection &current_connection, std::string &header_o);
int	header_extract(Connection &current_connection, std::string &header_o);
int	request_line(Server &server, Connection &current_connection);
int	request_header(Server &server, Connection &current_connection);
int	set_errNbr(Connection &current_connection, int nbr,
			int isReadingHeader, int isReadingBody, int isWriting);
int	socket_read(Connection &current_connection, std::vector<int> &to_del);
int	connection_read(Server &server, Connection &current_connection,
                std::vector<int> &to_del);
void	load_config_n_socket_create(int ac, char **av, std::vector<Server> &servers, int &max_fd);
std::string	get_status_code(int nbr);
void	load_config(int ac, char **av, std::vector<server_t> &server_config);
int	connection_write(Server &server, Connection &current_connection, std::vector<int> &to_del);


std::string handle_200(void);
std::string	handle_204(void);
std::string	handle_400(void);
std::string	handle_401(void);
std::string	handle_403(void);
std::string	handle_404(void);
std::string	handle_408(void);
std::string	handle_409(void);
std::string	handle_411(void);
#endif