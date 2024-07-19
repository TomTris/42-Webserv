
#include "webserv.hpp"

int my_atoi(std::string numb)
{
    std::stringstream ss(numb);
    int num;

    ss >> num;
    return num;
}

int get_ip_as_number(std::string add)
{
    int out = 0;
    for (int i = 0; i < 3; i++)
    {
        std::string temp = add.substr(0, add.find("."));
        add = add.substr(add.find(".") + 1, add.size());
        out <<= 8;
        out |= my_atoi(temp);
    }
    out <<= 8;
    out |= my_atoi(add);
    return out;
}

void set_bytes_to_zero(void *start, int len)
{
    char *transformed = reinterpret_cast<char *>(start);

    for (int i = 0; i < len; i++)
        transformed[i] = 0;
}

void createIPv4address(sockaddr_in* address, std::string ip, int port)
{
    set_bytes_to_zero(address, sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if (ip.size())
        address->sin_addr.s_addr = htonl(get_ip_as_number(ip));
    else
        address->sin_addr.s_addr = INADDR_ANY;
}

int createSocket()
{
     return (socket(AF_INET, SOCK_STREAM, 0));
}