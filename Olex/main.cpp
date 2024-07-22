

#include "webserv.hpp"



int main()
{
    std::vector<server_t> s;
    if (parse("/Users/obrittne/Desktop/webSerc/Configs/default.conf", s))
        return 1;
    std::vector<Server> servers;
    for (int i = 0; i < s.size(); i++)
    {
        servers.push_back(Server(s[i]));
    }
    servers[1].launch();
}
//test