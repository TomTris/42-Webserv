#include "Tomweb.hpp"

int	main(int ac, char **av, char **env)
{
	std::vector<Server>			servers;
	std::vector<struct pollfd> fds;
	int							activity;
	int							re_try;

	std::vector<int> a;

	re_try = 1000;

	try {
		load_config_n_socket_create(ac, av, servers);
		poll_reset(fds);
		add_servers_to_pool(servers, fds);
		usleep(500000);
		while(1)
		{
			activity = poll(fds.data(), fds.size(), 500);
			if (activity < 0)
			{
				perror("select error");
				if (--re_try == 0)
					return (0);
			}
			else
			{
				server_level(servers, fds);
				// connection_level(servers, fds);
				// read_level(servers, fds);
				// write_level(servers, fds);
				poll_reset(fds);
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
