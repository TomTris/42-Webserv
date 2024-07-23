
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../webserv.hpp"

#define LISTEN 10

struct location;
struct server_t;

class Server
{
    private:

    public:
        std::vector<location> locations;
        struct sockaddr_in address;
        std::map<int, std::string> errorPages;
        int err;
        int serverFd;
        int body_size_max;
        Server(server_t& s);
        
        ///
        void launch();
        void handleClient(int clientSocket);
        int set_non_blocking(int fd);
};

#endif