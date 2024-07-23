#include "../Tomweb.hpp"

Connection::Connection(int socket_fd_o)
{
    this->socket_fd = socket_fd_o;
    this->isFinished = 0;
    this->method = "";
    this->URL = "";
    this->HTTP_version = "";
    this->any_error = 0;
    this->circle_read = -1;
    this->circle_write = -1;
    this->content_length_left = -1;
    this->infile = -1;
    this->isWaiting = 1;
    this->isReading = 0;
    this->isWriting = 0;
}
Connection::~Connection()
{
    if (close(this->socket_fd) == -1)
        throw std::runtime_error("close failed in Connection");
}
