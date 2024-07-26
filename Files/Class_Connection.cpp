#include "../Tomweb.hpp"

Connection::~Connection()
{
}

Connection::Connection(int socket_fd)
	: socket_fd(socket_fd)
{
	this->IsAfterResponseClose = 0;
	this->isReadingHeader = 1;
	this->isWriting = 0;
	this->fdWritingTo = -1;
	this->fdReadingFrom = -1;
	this->doesClientClosed = 0;
	this->contentLength = 0;
	this->host = "";
	this->contentType = "";
	this->contentDisposition = "";
	this->boundary = "";
	this->form_name = "";
	this->file_name = "";
	this->method = "";
	this->URI = "";
	this->HTTP_version = "";
	this->errNbr = 0;
	this->circleTotal = 0;
	this->circleRead = 0;
	this->circleWrite = 0;
	this->have_read = "";
	
	
	
	this->autoIndex = 1;// change with config
}

void Connection::reset()
{
	this->IsAfterResponseClose = 0;
	this->isReadingHeader = 1;
	this->isWriting = 0;
	this->fdWritingTo = -1;
	this->fdReadingFrom = -1;
	this->doesClientClosed = 0;
	this->contentLength = 0;
	this->host = "";
	this->contentType = "";
	this->contentDisposition = "";
	this->boundary = "";
	this->form_name = "";
	this->file_name = "";
	this->method = "";
	this->URI = "";
	this->HTTP_version = "";
	this->errNbr = 0;
	this->circleTotal = 0;
	this->circleRead = 0;
	this->circleWrite = 0;
	
	
	
	this->autoIndex = 1;// change with config
}

