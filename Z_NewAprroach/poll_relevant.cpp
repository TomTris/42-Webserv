#include "Tomweb.hpp"
#include <poll.h>

void	add_to_poll(std::vector<struct pollfd> &fds, int fd_add, int option);
void	add_servers_to_pool(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	struct pollfd pfd;

	for (int i = 0; i < servers.size(); i++)
	{
		add_to_poll(fds, servers[i].serverFd, POLLIN);
	}
}

void	remove_from_poll(int fd_rm, std::vector<struct pollfd> &fds)
{
	if (fd_rm < 0)
		return ;
	for (int i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd_rm)
		{
			fds.erase(fds.begin() + i);
			return ;
		}
	}
}

void	add_to_poll(std::vector<struct pollfd> &fds, int fd_add, int option)
{
	struct pollfd pfd;
	// std::cout << "fd " << fd_add << " is add to " << option << std::endl;
	fds.push_back(pfd);
	(fds.end() - 1)->fd = fd_add;
	(fds.end() - 1)->events = option;
	(fds.end() - 1)->revents = 0;
}

void	change_option_poll(std::vector<struct pollfd> &fds, int fd, int option)
{
	// std::cout << "fd = " << fd << std::endl;
	for (int i = 0; i < fds.size(); i++)
	{
        if (fds[i].fd == fd) {
            fds[i].events = option;
			return ;
        }
    }
}

void poll_reset(std::vector<struct pollfd> &fds)
{
	for (int i = 0; i < fds.size(); i++)
	{
		fds[i].revents = 0;
	}
}

int check_fds(std::vector<struct pollfd> &fds, int fd) {
	if (fd == -1)
		return (-1);
    for (int i = 0; i < fds.size(); i++)
	{
        // std::cout << "fds[" << i << "] = " << fds[i].fd << std::endl;
		if (fds[i].fd == fd) {
			return fds[i].revents;
        }
    }
    // std::cerr << "sth WRONG in check_fds, fd " << fd << "not found" << std::endl;
    return -1;
}
