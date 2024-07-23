#include "../../Tomweb.hpp"

Server::Server(server_t& s): err(0)
{
    this->errorPages = s.errorpages;
    this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverFd < 0)
    {
        this->err = 1;
        return ;
    }
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(s.port);
    this->address.sin_addr.s_addr = htonl(s.host);
    this->locations = s.locations;
    this->body_size_max = s.client_max_body_size;
    if (bind(this->serverFd, (sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        close(this->serverFd);
        this->err = 1;
        return ;    
    }
    if (listen(this->serverFd, 10) < 0)
    {
        close(this->serverFd);
        this->err = 1;
        return ;
    }
}

Server::~Server() {
}
