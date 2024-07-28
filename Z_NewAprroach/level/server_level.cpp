#include "../Tomweb.hpp"

void	server_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	int	add;
	std::cout << "server level" << std::endl;
	for (int i = 0; i < servers.size(); i++)
	{
		if (check_fds(fds, servers[i].serverFd) == POLLIN)
		{
			add = connection_accept(servers[i], fds);
			if (add != -1)
			{
				servers[0].connections.push_back(add);
				add_to_poll(fds, add, POLLIN | POLLOUT);
			}
		}
	}
}
