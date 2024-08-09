#include "../Tomweb.hpp"

int	connection_accept(Server &server)
{
	int		new_socket;
	size_t	addrlen = sizeof(server.address);

	if ((new_socket = accept(server.serverFd, (struct sockaddr *)&server.address, (socklen_t*)&addrlen)) <= 0)
		return (perror("accept failed"), 1);
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
		return (perror("fcnl failed"), -1);
	
	Connection a(new_socket);
	server.connections.push_back(a);
	add_to_poll(new_socket, POLLIN);
	return 1;
}

void	server_level(std::vector<Server> &servers)
{
	for (unsigned int i = 0; i < servers.size(); i++)
	{
		if (check_fds(servers[i].serverFd) == POLLIN)
		{
			connection_accept(servers[i]);
		}
	}
}
