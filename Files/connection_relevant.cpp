#include "Tomweb.hpp"

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
		connection_delete(fd_to_del, servers);
	}
}

void	connection_accept(Server &server)
{
	int		new_socket;
	size_t	addrlen = sizeof(server.address);

	if ((new_socket = accept(server.serverFD, (struct sockaddr *)&server.address, (socklen_t*)&addrlen)) < 0)
		throw std::runtime_error("accept failed");
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl failed");
	server.to_add_fds.pushback(new_socket);

}

void connections_add(std::vector<Server> &servers)
{
	for (int i = 0; i < servers.size(); i++)
	{
		while (servers.to_add_fds.size() > 0)
		{
			servers.connections.push_back(Connection(servers.to_add_fds[0]));
			to_add_fds.erase(to_add_fds.begin());
		}
	}
}

void	connection_exception(Connection &current_connection)
{
	current_connection.isError = 1;
}

void	header_get(Connection &current_connection)
{
    char buffer[50] = {0};

    if (read(new_socket, buffer, sizeof(buffer) - 1) > 0) {
        current_connection = 
        if (bytes_read < sizeof(buffer) - 1)
			break;
    }
    
    if (bytes_read == -1) {
        throw std::runtime_error("read failed");
    }    
}

void	connection_read(Connection &current_connection, std::vector<int> &to_del)
{
	if (current_connection.requests.size() == 0)
	{
		header_get(current_connection, to_del);
	}
}