#include "../Tomweb.hpp"

int	connection_accept(Server &server)
{
	int		new_socket;
	size_t	addrlen = sizeof(server.address);

	if ((new_socket = accept(server.serverFd, (struct sockaddr *)&server.address, (socklen_t*)&addrlen)) < 0)
	{
		perror("accept failed");
		return -1;
	}
	int a = check_fds(new_socket);
	if (a == -1)
	{
		if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
		{
			perror("fcnl failed");
			return -1;
		}
		server.connections.push_back(Connection(new_socket));
		std::cout << "ACCEEPPTT" << new_socket <<  std::endl;
		add_to_poll(new_socket, POLLOUT);
	}
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
