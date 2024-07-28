#include "Tomweb.hpp"

int	main(int ac, char **av, char **env)
{
	std::vector<Server>			servers;
	std::vector<struct pollfd> fds;
	int							activity;
	int							re_try;

	std::vector<int> a;

	re_try = 1000;
	std::cout << "POLL_IN = " << POLL_IN << std::endl;
	std::cout << "POLL_OUT = " << POLL_OUT << std::endl;
	// int	fd = open("./Makefile", O_RDONLY);
	// std::cout << "fd there = " << fd << std::endl;
	// add_to_poll(fds, fd, POLLIN|POLLOUT);
	try {
		load_config_n_socket_create(ac, av, servers);
		add_servers_to_pool(servers, fds);
		while(1)
		{
			activity = poll(fds.data(), fds.size(), 1000);
			std::cout << "fds.size = " << fds.size() << std::endl;
			// std::cout << "fd here = " << fd << std::endl;
			usleep(1000000);
			// check_fds(fds, fd);
			if (activity < 0)
			{
				perror("select error");
				if (--re_try == 0)
					return (0);
			}
			else
			{
				server_level(servers, fds);
				connection_level(servers, fds);
				read_level(servers, fds);
				write_level(servers, fds);
				std::cout << "------" << std::endl;
			}
		}
	}
	catch (const std::runtime_error&e) {
		std::cout << "throw called"<<std::endl;
		perror(e.what());
		return (0);
	}
	std::cout << "hhmm"<<std::endl;
	return (0);
}
