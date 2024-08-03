#include "../Tomweb.hpp"

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
    this->locations.push_back(s.locations);
    this->server_names.push_back(s.servername);
    this->body_size_max = s.client_max_body_size;
    int opt = 1;
	if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) {
		close(this->serverFd);
        this->err = 1;
        throw std::runtime_error("Set Address Reuse Failed");
	}
    if (bind(this->serverFd, (sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        close(this->serverFd);
        this->err = 1;
        throw std::runtime_error("bind");
    }
    if (listen(this->serverFd, 128) < 0)
    {
        close(this->serverFd);
        this->err = 1;
        throw std::runtime_error("listen");
    }
    this->host = s.host;
    this->port = s.port;
}

Server::~Server() {
}

std::string Server::return_default(int numb)
{
    std::map<int, std::string> mp;
    mp[200] = ERROR200;
    mp[204] = ERROR204;
    mp[400] = ERROR400;
    mp[401] = ERROR401;
    mp[402] = ERROR402;
    mp[403] = ERROR403;
    mp[404] = ERROR404;
    mp[405] = ERROR405;
    mp[408] = ERROR408;
    mp[409] = ERROR409;
    mp[411] = ERROR411;
    mp[431] = ERROR431;
    return (mp[numb]);
}


std::string Server::get_error_page(int numb)
{
    if (this->errorPages.find(numb) == this->errorPages.end())
        return (return_default(numb));
    std::string err = this->errorPages[numb];
    if (checkIfFileExistsAndNotDirectory(err))
    {
        return (err);
    }
    return (return_default(numb));
}
