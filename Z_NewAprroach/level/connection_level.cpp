#include "../Tomweb.hpp"

void	connection_level(std::vector<Server> &servers, std::vector<struct pollfds> &fds)
{
	for (int i = 0; i < servers.size(); i++)
	{
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			if (servers[i].connections[j].readingHeaderDone == 0 && check_fds(fds, servers[i].connections[j].socket_fd) == POLLIN)
			{
				reading_header(servers[i], servers[i].connections[j], fds);
			}
		}
	}
}
int check_fds(std::vector<struct pollfd> &fds, int fd)