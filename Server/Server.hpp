
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../webserv.hpp"

#define LISTEN 10

struct location;
struct server_t;

class Server
{
    private:
        struct sockaddr_in address;
        int                opt;

    public:
        std::vector<location> locations;
        int err;
        int serverFd;
        Server(server_t& s);
        void launch();
        void handleClient(int clientSocket);
};

#endif