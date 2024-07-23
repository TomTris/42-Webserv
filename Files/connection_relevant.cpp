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

void	connection_exception(Connection &current_connection, std::vector<int> &to_del)
{
	current_connection.isError = 1;
	std::cout << "Connection with fd = {" << current_connection.socket_fd << "} has exception" << std::endl;
	to_del.push_back(current_connection.socket_fd);
}

void	set_any_error(Connection &current_connection, int nbr)
{
	current_connection.any_error = nbr;
	current_connection.isWaiting = 0;
	current_connection.isReading = 0;
	current_connection.isWriting = 1;
}

void	header_get(Connection &current_connection, std::vector<int> &to_del)
{
    char	buffer[50] = {0};

    if (read(new_socket, buffer, sizeof(buffer) - 1) < 0)
	{
		perror("read Failed?");
		to_del.push_back(current_connection.socket_fd);
		return ;
	}
	
	have_read += buffer;
	if (have_read.length() == 0)
	{
		std::cerr << "Client closes the connection" << std::endl;
		to_del.push_back(current_connection.socket_fd);
		return ;
	}
}

void	header_check(Server &server, Connection &current_connection, std::vector<int> &to_del)
{
	ssize_t request_line_endl;

	if (have_read.length() > 40)
		return (set_any_error(current_connection, 414));
	
	request_line_end = have_read.find("\r\n");
	if (request_line_end == std::string::npos)
		return (set_any_error(current_connection, 400));

	std::string request_line = have_read.substr(0, request_line_end);
	std::stringstream socket_stream(request_line);

	if (!(socket_stream >> method >> URL >> HTTP_version) || HTTP_version != "HTTP/1.1" !! URL == "")
		return (set_any_error(current_connection, 400));
	location loc = get_location(server.locations, URL);
	URL = get_path_to_file(loc, URL);
	if (!isAllowed(loc, method))
		return (set_any_error(current_connection, 400));
}
std::string get_path_to_file(location& loc, std::string path)


void	connection_read(Server &server, Connection &current_connection, std::vector<int> &to_del)
{
	if (current_connections.size() == 0)
	{
		current_connection.push_back();
	}

	if (current_connection.isWaiting == 1)
	{
		current_connection.isWaiting = 0;
		current_connection.isReading = 1;
		header_get(server, current_connection, to_del);
		header_check(server, current_connection, to_del);
	}
}