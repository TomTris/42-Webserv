#include "../Tomweb.hpp"
#include <poll.h>

void	add_to_poll(int fd_add, int option);
void	add_servers_to_pool(std::vector<Server> &servers)
{

	for (unsigned int i = 0; i < servers.size(); i++)
	{
		add_to_poll(servers[i].serverFd, POLLIN);
	}
}

void	remove_from_poll(int fd_rm)
{
	if (fd_rm < 0)
		return ;
	for (unsigned int i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd_rm)
		{
			fds.erase(fds.begin() + i);
			return ;
		}
	}
}

void	add_to_poll(int fd_add, int option)
{
	struct pollfd pfd;
	pfd.fd = fd_add;
	pfd.events = option;
	pfd.revents = 0;
	fds.push_back(pfd);
}

void	change_option_poll(int fd, int option)
{
	for (unsigned int i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd) {
			fds[i].events = option;
			return ;
		}
	}
}

void revents_to_0(int fd)
{
	if (fd == -1)
		return ;
	// for (unsigned int i = 0; i < fds.size(); i++)
	// {
	// 	if (fds[i].fd == fd)
	// 	{
	// 		fds[i].revents = 0;
	// 		return ;
	// 	}
	// }
}

int check_fds(int fd)
{
	if (fd == -1)
		return (-1);
	for (unsigned int i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd)
		{
			return fds[i].revents;
		}
	}
	return -1;
}

