#include "../Tomweb.hpp"

int	connection_accept(Server &server, std::vector<struct pollfd> &fds)
{
	int		new_socket;
	size_t	addrlen = sizeof(server.address);
	// std::cout << "NEW CONNECTION COMING" << std::endl;
	// std::cout << "NEW CONNECTION COMING" << std::endl;
	// std::cout << "NEW CONNECTION COMING" << std::endl;
	// std::cout << "NEW CONNECTION COMING" << std::endl;
	if ((new_socket = accept(server.serverFd, (struct sockaddr *)&server.address, (socklen_t*)&addrlen)) < 0)
	{
		perror("accept failed");
		return -1;
	}
	int a = check_fds(fds, new_socket);
	// std::cout << " a = " << a << std::endl;
	if (a == -1)
	{
		if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
		{
			perror("fcnl failed");
			return -1;
		}
		server.connections.push_back(Connection(new_socket));
		std::cout << "ACCEEPPTT" << new_socket<< std::endl;
	}
	else
	{
		// std::cout << "BUT I's THE OLD ONE" << std::endl;
		return 1;
	}
	add_to_poll(fds, new_socket, POLLIN);
	return (new_socket);
}

void	server_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	for (int i = 0; i < servers.size(); i++)
	{
		// std::cout << "server " << i << " has " << servers[i].connections.size() << " connections" << std::endl;
		if (check_fds(fds, servers[i].serverFd) == POLLIN)
		{
			connection_accept(servers[i], fds);
		}
		// std::cout << "server " << i << " has " << servers[i].connections.size() << " connections" << std::endl;
	}
}
