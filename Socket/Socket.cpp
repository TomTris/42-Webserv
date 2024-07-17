#include "Socket.hpp"
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/event.h>

Socket::Socket(int domain, int service, int protocol, unsigned long interface, int port, int backlog)
{
    this->domain = domain;
    this->service = service;
    this->protocol = protocol;
    this->interface = interface;
    this->port = port;
    this->backlog = backlog;
    this->err = 0;

    // Zero-initialize the address structure
    memset(&(this->address), 0, sizeof(this->address));

    this->address.sin_family = domain;
    this->address.sin_port = htons(port);
    this->address.sin_addr.s_addr = htonl(interface);
    
    this->socketFd = socket(domain, service, protocol);
    if (this->socketFd == -1)
    {
        std::cerr << "Socket creation failed" << std::endl;
        this->err = 1;
        return;
    }
    
    if (bind(this->socketFd, (struct sockaddr *)(&(this->address)), sizeof(this->address)) != 0)
    {
        std::cerr << "Bind failed" << std::endl;
        this->err = 1;
        close(this->socketFd);
        return;
    }
    
    if (listen(this->socketFd, this->backlog) != 0)
    {
        std::cerr << "Listen failed" << std::endl;
        this->err = 1;
        close(this->socketFd);
        return;
    }
}

void Socket::launch(void)
{
    int kq = kqueue();
    if (kq == -1)
    {
        std::cerr << "kqueue creation failed" << std::endl;
        return;
    }

    struct kevent change;
    EV_SET(&change, this->socketFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1)
    {
        std::cerr << "kevent setup failed" << std::endl;
        close(kq);
        return;
    }

    std::string hello = "HTTP/1.1 200 OK\r\n"
                        "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
                        "Server: Apache/2.2.14 (Win32)\r\n"
                        "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
                        "Content-Length: 88\r\n"
                        "Content-Type: text/html\r\n"
                        "Connection: Closed\r\n"
                        "\r\n"
                        "<html><body><h1>Hello, World!</h1></body></html>";

    struct kevent events[10];

    while (true)
    {
        int nevents = kevent(kq, NULL, 0, events, 10, NULL);
        if (nevents == -1)
        {
            std::cerr << "kevent error" << std::endl;
            break;
        }

        for (int i = 0; i < nevents; ++i)
        {
            if (events[i].ident == this->socketFd)
            {
                // Accept new connection
                int newSocket = accept(this->socketFd, NULL, NULL);
                if (newSocket == -1)
                {
                    std::cerr << "Accept failed" << std::endl;
                    continue;
                }

                // Add new socket to kqueue
                struct kevent newChange;
                EV_SET(&newChange, newSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                if (kevent(kq, &newChange, 1, NULL, 0, NULL) == -1)
                {
                    std::cerr << "kevent add new socket failed" << std::endl;
                    close(newSocket);
                }
            }
            else
            {
                // Handle read event
                char buffer[30000] = {0};
                int valread = read(events[i].ident, buffer, 30000);
                if (valread < 0)
                {
                    std::cerr << "Read failed" << std::endl;
                    close(events[i].ident);
                    continue;
                }

                std::cout << buffer << std::endl;

                int valsend = write(events[i].ident, hello.c_str(), hello.size());
                if (valsend < 0)
                {
                    std::cerr << "Write failed" << std::endl;
                }

                close(events[i].ident);
            }
        }
    }

    close(kq);
}
