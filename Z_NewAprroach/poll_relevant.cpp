#include "../Tomweb.hpp"
#include <poll.h>

void	add_servers_to_pool(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	struct pollfd pfd;

	for (int i = 0; i < servers.size(); i++)
	{
		fds.push_back(pfd);
		fds[i].fd = servers[i].serverFd;
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}
}

void	remove_from_poll(int fd_rm, std::vector<struct pollfd> &fds)
{
	for (int i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd_rm)
		{
			fds.erase(fds.begin() + i);
			return ;
		}
	}
	std::cout << "sth wrong with remove_form_poll" << std::endl;
}

void	add_to_poll(std::vector<struct pollfd> &fds, int fd_add, int option)
{
	struct pollfd pfd;
	std::cout << "a" << std::endl;
	fds.push_back(pfd);
	std::cout << "b" << std::endl;
	(fds.end() - 1)->fd = fd_add;
	(fds.end() - 1)->events = option;
	(fds.end() - 1)->revents = 0;
}

void	change_option_poll(std::vector<struct pollfd> &fds, int fd, int option)
{
	for (int i = 0; i < fds.size(); i++)
	{
        if (fds[i].fd == fd) {
            fds[i].events = option;
			return ;
        }
    }
	std::cerr << "sth wrong in change_option_poll" << std::endl;
    // return POLLERR; // Indicate error if the fd is not found
}

void poll_reset(std::vector<struct pollfd> &fds)
{
	for (int i = 0; i < fds.size(); i++)
	{
		fds[i].revents = 0;
	}
}

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
	std::cout << "AAAACCCCEEEEPPPPTTTT; "<< new_socket << std::endl;
	return (new_socket);
}

int check_fds(std::vector<struct pollfd> &fds, int fd) {
    // Use const_iterator for read-only access
    for (int i = 0; i < fds.size(); i++)
	{
        if (fds[i].fd == fd) {
            return fds[i].revents;
        }
    }
    std::cerr << "sth wrong in check_fds" << std::endl;
    return POLLERR; // Indicate error if the fd is not found
}


// void connections_add(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
// {
// 	struct pollfd pfd;

// 	for (int i = 0; i < servers.size(); i++)
// 	{
// 		while (servers[i].to_add_fds.size() > 0)
// 		{
// 			servers[i].connections.push_back(Connection(servers[i].to_add_fds[0]));
// 			fds.push_back(pfd);
// 			(fds.end() - 1)->fd = servers[i].to_add_fds[0];
// 			(fds.end() - 1)->events = POLLIN;
// 			(fds.end() - 1)->revents = 0;
// 			servers[i].to_add_fds.erase(servers[i].to_add_fds.begin());
// 		}
// 	}
// }


// int	connection_delete(int &fd_to_del, std::vector<Server> &servers)
// {
// 	for (int i = 0; i < servers.size(); i++)
// 	{
// 		for (int j = 0; j < servers[i].connections.size(); j++)
// 		{
// 			if (servers[i].connections[j].socket_fd == fd_to_del)
// 			{
// 				servers[i].connections.erase(servers[i].connections.begin() + j);
// 				return (1);
// 			}
// 		}
// 	}
// 	std::cout << "connection delete somehow wrong" << std::endl;
// 	return (0);
// }

// int	connections_delete(std::vector<int> &to_del_fds, std::vector<Server> &servers)
// {
// 	while (to_del_fds.size() > 0)
// 	{
// 		std::cout << "rm socket " << to_del_fds[0] << std::endl;
// 		connection_delete(to_del_fds[0], servers);
// 		close(to_del_fds[0]);
// 		to_del_fds.erase(to_del_fds.begin());
// 	}
// 	return (1);
// }
