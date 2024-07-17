
#include "webserv.hpp"

void handleClient(int clientSocket)
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

int main()
{
    int serverSocketFd = createSocket();
    struct sockaddr_in serverAddress;
    createIPv4address(&serverAddress, "127.0.0.1", 2000);
    if (bind(serverSocketFd, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Bind error\n";
        return 1;
    }

    if (listen(serverSocketFd, 10) < 0)
    {
        std::cerr << "Listen error\n";
        return 1;
    }

    std::cout << "Server is listening on port 2000\n";

    while (true)
    {
        struct sockaddr_in clientAddress;
        socklen_t sizeOfclient = sizeof(clientAddress);
        int clientSocket = accept(serverSocketFd, (sockaddr *)&clientAddress, &sizeOfclient);

        if (clientSocket < 0)
        {
            std::cerr << "Accept error\n";
            continue;
        }

        handleClient(clientSocket);
    }

    close(serverSocketFd);
    return 0;
}