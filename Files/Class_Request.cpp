#include "../Tomweb.hpp"

Request::Request(int *isError_o) : isError(isError_o)
{
    this->isFinished = 0;
    this->method = "";
    this->URL = "";
    this->HTTP_version = "";
    this->any_error = 0;
    this->circle_read = -1;
    this->circle_write = -1;
    this->content_length_left = -1;
    this->infile = -1;
}