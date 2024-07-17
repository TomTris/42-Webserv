
#include "Server.hpp"

Server::Server(server_t& s):err(0)
{
    this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverFd < 0)
    {
        this->err = 1;
        return ;
    }
    set_bytes_to_zero(&this->address, sizeof(this->address));
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(s.port);
    this->address.sin_addr.s_addr = htonl(s.host);
    this->locations = s.locations;
    if (bind(this->serverFd, (sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        close(this->serverFd);
        this->err = 1;
        return ;    
    }
    if (listen(this->serverFd, LISTEN) < 0)
    {
        close(this->serverFd);
        this->err = 1;
        return ;
    }
}

void Server::handleClient(int clientSocket)
{
    const int bufferSize = 1024;
    char buffer[bufferSize];
    recv(clientSocket, buffer, bufferSize, 0);
    
    std::string request(buffer);
    std::cout << "Received request:\n" << request << "\n";

    // Simple HTTP response
    std::string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>Simple HTTP Server</title></head>"
        "<body><h1>Hello from the simple HTTP server!</h1></body>"
        "</html>";
    
    send(clientSocket, response.c_str(), response.size(), 0);
    close(clientSocket);
}

void Server::launch()
{
    while (true)
    {
        struct sockaddr_in clientAddress;
        socklen_t sizeOfclient = sizeof(clientAddress);
        int clientSocket = accept(this->serverFd, (sockaddr *)&clientAddress, &sizeOfclient);

        if (clientSocket < 0)
        {
            std::cerr << "Accept error\n";
            continue;
        }

        handleClient(clientSocket);
    }
}