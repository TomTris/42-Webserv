#include "Tomweb.hpp"
#include <signal.h>

std::vector<struct pollfd>  fds;
int	main(int ac, char **av)
{
	std::vector<Server>			servers;
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	if (BUFFERSIZE > 10000)
		return (std::cerr << "Too big BUFFERSIZE" << std::endl, 1);
	if (BUFFERSIZE < 2)
		return (std::cerr << "Invalid BUFFERSIZE" << std::endl, 1);
	try
	{
		load_config_n_socket_create(ac, av, servers);
		add_servers_to_pool(servers);
		std::cout << "ok " << std::endl;
		
		while(1)
		{
			// check_fds(0);
			// sleep(2);
			poll(fds.data(), fds.size(), 0);
			server_level(servers);

			poll(fds.data(), fds.size(), 0);
			connection_level(servers);

			poll(fds.data(), fds.size(), 0);
			read_level(servers);

			poll(fds.data(), fds.size(), 0);
			write_level(servers);
		}
	}
	catch (const std::runtime_error&e)
	{
		std::cerr << "Throw called"<<std::endl;
		perror(e.what());
		return (0);
	}
	std::cout << "hhmm"<<std::endl;
	return (0);
}
