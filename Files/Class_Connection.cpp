#include "../Tomweb.hpp"

Connection::Connection(int socket_fd_o)
{
    this->socket_fd = socket_fd_o;
}
Connection::~Connection()
{
    if (close(this->socket_fd) == -1)
        throw std::runtime_error("close failed in Connection");
}
