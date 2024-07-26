#include "../Tomweb.hpp"

void	handle_event(std::vector<Server> &servers, fd_set &read_fds, fd_set &write_fds, std::vector<int> &to_del)
{
	int					fd;
	Connection			*current_connection;

	for (int i = 0; i < servers.size(); i++)
	{
		std::cout << "Getin serv 1" << std::endl;
		if (FD_ISSET(servers[i].serverFd, &read_fds))
		{
			connection_accept(servers[i]);
		}
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			std::cout << "Getin serv 2" << std::endl;
			current_connection = &(servers[i].connections[j]);
			if (FD_ISSET(current_connection->socket_fd, &read_fds) && current_connection->isWriting == 0)
			{
				std::cout << "Getin serv 3" << std::endl;
				// std::cout << "++++++++1+++++++" << std::endl;
				connection_read(servers[i], *current_connection, to_del);
				// std::cout << "+++++++2++++++++" << std::endl;
			}
			else if (FD_ISSET(current_connection->socket_fd, &write_fds))
			{
				std::cout << "Getin serv 4" << std::endl;
				// std::cout << "++++++++3+++++++" << std::endl;
				connection_write(servers[i], *current_connection, to_del);
				// std::cout << "+++++++++4++++++" << std::endl;
			}
		}
	}
}

void	set_init(fd_set &read_fds, fd_set &write_fds, int &max_fd, std::vector<Server> servers)
{
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	max_fd = 0;
	for (int i = 0; i < servers.size(); i++)
	{
		FD_SET(servers[i].serverFd, &read_fds);
		if (servers[i].serverFd > max_fd)
			max_fd = servers[i].serverFd;
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			FD_SET(servers[i].connections[j].socket_fd, &read_fds);
			FD_SET(servers[i].connections[j].socket_fd, &write_fds);
			if (servers[i].connections[j].socket_fd > max_fd)
				max_fd = servers[i].connections[j].socket_fd;
		}
	}
}

int	main(int ac, char **av)
{
	std::vector<Server>			servers;
	fd_set						read_fds;
	fd_set						write_fds;
	struct timeval				tv;
	std::vector<int> 			to_del_fds;
	int							max_fd;
	int							activity;
	int							re_try;

	re_try = 1000;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	try {
		FD_ZERO(&read_fds);
		load_config_n_socket_create(ac, av, servers, max_fd);
		while (1)
		{
			set_init(read_fds, write_fds, max_fd, servers);
			activity = select(max_fd + 1, &read_fds, &write_fds, NULL, NULL);
			usleep(500000);
			if (activity < 0  && errno != EINTR)
			{
				// std::cout << "activity = " << activity << std::endl;
				perror("select error");
				if (--re_try == 0)
				{
					return (0);
				}
			} 
			else
			{
				re_try = 1000;
				handle_event(servers, read_fds, write_fds, to_del_fds);
				// std::cout << "--------1---------" << std::endl;
				connections_add(servers);
				// std::cout << "--------2---------" << std::endl;
				connections_delete(to_del_fds, servers);
				// std::cout << "--------3---------" << std::endl;
			}
		}
	}
	catch (const std::runtime_error&e) {
		std::cout << "123123"<<std::endl;
		perror(e.what());
		return (0);
	}
	std::cout << "123123aaa"<<std::endl;
	return (0);
}
