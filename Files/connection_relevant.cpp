#include "../Tomweb.hpp"

int	connection_delete(int &fd_to_del, std::vector<Server> &servers)
{

	for (int i = 0; i < servers.size(); i++)
	{
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			if (servers[i].connections[j].socket_fd == fd_to_del)
			{
				servers[i].connections.erase(servers[i].connections.begin() + j);
				return (1);
			}
		}
	}
	std::cout << "connection delete somehow wrong" << std::endl;
	return (0);
}

int	connections_delete(std::vector<int> &to_del_fds, std::vector<Server> &servers)
{
	while (to_del_fds.size() > 0)
	{
		connection_delete(to_del_fds[0], servers);
	}
	return (1);
}

void	connection_accept(Server &server)
{
	int		new_socket;
	size_t	addrlen = sizeof(server.address);

	if ((new_socket = accept(server.serverFd, (struct sockaddr *)&server.address, (socklen_t*)&addrlen)) < 0)
		throw std::runtime_error("accept failed");
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl failed");
	server.to_add_fds.push_back(new_socket);

}

void connections_add(std::vector<Server> &servers)
{
	for (int i = 0; i < servers.size(); i++)
	{
		while (servers[i].to_add_fds.size() > 0)
		{
			servers[i].connections.push_back(Connection(servers[i].to_add_fds[0]));
			servers[i].to_add_fds.erase(servers[i].to_add_fds.begin());
		}
	}
}
