#include "Tomweb.hpp"
#include <signal.h>
int	main(int ac, char **av)
{
	std::vector<Server>			servers;
	std::vector<struct pollfd>  fds;
	// int							activity;
	int							re_try;
	// signal()
	re_try = 1000;
	signal(SIGPIPE, SIG_IGN);
	if (BUFFERSIZE > 10000)
		return (std::cerr << "Too big BUFFERSIZE" << std::endl, 1);
	if (BUFFERSIZE < 2)
		return (std::cerr << "Invalid BUFFERSIZE" << std::endl, 1);
	try
	{
		load_config_n_socket_create(ac, av, servers);
		add_servers_to_pool(servers, fds);
		std::cout << "ok " << std::endl;
		while(1)
		{
			// activity = poll(fds.data(), fds.size(), 0);
			// if (activity < 0)
			// {
			// 	perror("select error");
			// 	if (--re_try == 0)
			// 		return (0);
			// }
			// else
			// {
				check_fds(fds, 0);
				// std::cout << "poll" << std::endl;
				poll(fds.data(), fds.size(), 1000);
				// std::cout << "server" << std::endl;
				server_level(servers, fds);

				// std::cout << "connection" << std::endl;
				connection_level(servers, fds);

				// std::cout << "poll" << std::endl;
				poll(fds.data(), fds.size(), 1000);

				// std::cout << "read_level" << std::endl;
				read_level(servers, fds);

				// std::cout << "poll" << std::endl;
				poll(fds.data(), fds.size(), 1000);
				
				// std::cout << "write_level" << std::endl;
				write_level(servers, fds);
				// std::cout << poll << std::endl;
				// if (errno != 0 && errno != 2)
				// {
				// 	perror("aa");
				// 	sleep(3);
				// }
				// std::cout <<5 << std::endl;
			// }
		}
	}
	catch (const std::runtime_error&e)
	{
		std::cerr << "throw called"<<std::endl;
		perror(e.what());
		return (0);
	}
	std::cout << "hhmm"<<std::endl;
	return (0);
}
