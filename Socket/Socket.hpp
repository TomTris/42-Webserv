
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <netinet/in.h>

class Socket
{
    public:
        int domain;
        int service;
        int protocol;
        unsigned long interface;
        int port;
        int backlog;
        struct sockaddr_in address;
        int socketFd;
        int err;

        Socket(int domain, int service, int protocol, unsigned long interface, int port, int backlog);
        void launch(void);
};

#endif