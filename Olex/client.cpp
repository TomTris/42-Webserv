

#include "webserv.hpp"



int main()
{
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }
    struct sockaddr_in address;
    createIPv4address(&address, "127.0.0.1", 2000);
    
    write(1, "asdasd", 5);
    if (!connect(socketFD, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)))
        write(1, "good", 4);
    else
        write(1, "not good", 9);
    std::string asd = "GET \\HTTP/1.1\r\nHost::google.com\r\n\r\n";
    send(socketFD, asd.c_str(), asd.size(), 0);
    char answer[1024];
    recv(socketFD, answer, 1024, 0);
    std::cout << answer;
    close(socketFD);
}