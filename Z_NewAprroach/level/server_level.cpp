#include "../Tomweb.hpp"

int	connection_accept(Server &server, std::vector<struct pollfd> &fds)
{
	int		new_socket;
	size_t	addrlen = sizeof(server.address);

	if ((new_socket = accept(server.serverFd, (struct sockaddr *)&server.address, (socklen_t*)&addrlen)) < 0)
	{
		perror("accept failed");
		return -1;
	}
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("fcnl failed");
		return -1;
	}
	server.connections.push_back(Connection(new_socket));
	add_to_poll(fds, new_socket, POLLIN);
	(server.connections.end() - 1)->time_out = clock();
	// std::cout << "ACCCEEPT " << new_socket << std::endl;
	return (new_socket);
}

void	server_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	for (int i = 0; i < servers.size(); i++)
	{
		if (check_fds(fds, servers[i].serverFd) == POLLIN)
		{
			connection_accept(servers[i], fds);
		}
	}
}
