#include "Tomweb.hpp"

int	main(int ac, char **av, char **env)
{
	std::vector<Server>			servers;
	std::vector<struct pollfd> fds;
	int							activity;
	int							re_try;
int	i = 0;
	std::vector<int> a;

	re_try = 1000;
	try {
		load_config_n_socket_create(ac, av, servers);
		add_servers_to_pool(servers, fds);
		while(1)
		{
			activity = poll(fds.data(), fds.size(), 0);
			std::cout << "fds.size = " << fds.size() << std::endl;
			usleep(2000);
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
				// std::cout << "--" << i++ << "----" << std::endl;
			}
		}
	}
	catch (const std::runtime_error&e) {
		std::cerr << "throw called"<<std::endl;
		perror(e.what());
		return (0);
	}
	// std::cout << "hhmm"<<std::endl;
	return (0);
}
