#include "../Tomweb.hpp"

// void	handle_event(std::vector<Server> &servers, fd_set &read_fds, fd_set &write_fds, std::vector<int> &to_del)
// {
// 	int					fd;
// 	Connection			*current_connection;

// 	for (int i = 0; i < servers.size(); i++)
// 	{
// 		if (FD_ISSET(servers.serversFD, &read_fds))
// 		{
// 			connection_accept(servers[i]);
// 		}
// 		for (int j = 0; j < servers[i].connection.size(); j++)
// 		{
// 			current_connection = &(servers[i].connection[j]);
// 			if (FD_ISSET(current_connection->socket_fd, &exception_fds))
// 			{
// 				connection_exception(current_connection);
// 			}
// 			if (FD_ISSET(current_connection->socket_fd, &read_fds))
// 			{
// 				connection_read(current_connection, to_del);
// 			}
// 			if (FD_ISSET(current_connection->socket_fd, &write_fds))
// 			{
// 				connection_write(current_connection);
// 			}
// 			connect_handle(server[i], current_connection);
// 		}
// 	}
// }

// void	set_init(fd_set &read_fds, fd_set &write_fds, int &max_fd, std::vector<Server> servers)
// {
// 	FD_ZERO(&read_fds);
// 	FD_ZERO(&write_fds);
// 	max_fd = 0;
// 	for (int i = 0; i < servers.size(); i++)
// 	{
// 		FD_SET(servers[i].serverFd, &read_fds);
// 		if (servers[i].serverFd > max_fd)
// 			max_fd = servers[i].serverFd;
// 		for (int j = 0; j < servers[i].connections.size(); j++)
// 		{
// 			if (servers[i].connections[j].busy == 0)
// 			{
// 				FD_SET(servers[i].connections[j].socket_fd, &read_fds);
// 				FD_SET(servers[i].connections[j].socket_fd, &write_fds);
// 				if (servers[i].connections[j].socket_fd > max_fd)
// 					max_fd = servers[i].connections[j].socket_fd;
// 			}
// 		}
// 	}
// }

// int	main(int ac, char **av)
// {
// 	std::vector<Server>			servers;
// 	fd_set						read_fds;
// 	fd_set						write_fds;
// 	fd_set						exception_fds;
// 	struct timeval				tv;
// 	std::vector<int> 			to_del_fds;
// 	int							max_fd;
// 	int							activity;

// 	tv.tv_sec = 0;
// 	tv.tv_usec = 100000;
// 	try {
// 		FD_ZERO(&read_fds);
// 		load_config_n_socket_create(ac, av, servers);
// 		while (1)
// 		{
// 			set_init(read_fds, write_fds, max_fd, servers);
// 			activity = select(max_fd + 1, &read_fds, &write_fds, &exception_fds, &tv);
// 			if (activity < 0  && errno != EINTR)
// 			{
// 				perror("select error");
// 				sleep(1);
// 			} 
// 			else
// 			{
// 				handle_event(servers, read_fds, write_fds, to_del_fds);
// 				connections_delete(to_del_fds, servers);
// 				connections_add(servers);
// 			}
// 		}
// 	}
// 	catch (const std::runtime_error&e) {
// 		close(server_data.server_fd);
// 		perror(e.what());
// 		return (1);
// 	}
// 	return (0);
// }

int	main(int ac, char **av)
{
	std::vector<Server>	servers;

	load_config_n_socket_create(ac, av, servers);
}