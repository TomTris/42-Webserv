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
				std::cout <<1 << std::endl;
				poll(fds.data(), fds.size(), 2);
				// if (errno != 0 && errno != 2)
				// {
				// 	perror("ab");
				// 	sleep(3);
				// }
				server_level(servers, fds);
				// if (errno != 0 && errno != 2)
				// {
				// 	perror("ac");
				// 	sleep(3);
				// }
				// std::cout <<2 << std::endl;
				connection_level(servers, fds);
				// if (errno != 0 && errno != 2)
				// {
				// 	perror("ae");
				// 	std::cout << "errno = " << errno << std::endl;
				// 	sleep(3);
				// }
				poll(fds.data(), fds.size(), 2);
				// std::cout <<3 << std::endl;
				read_level(servers, fds);
				// if (errno != 0 && errno != 2)
				// {
				// 	perror("af");
				// 	sleep(3);
				// }
				poll(fds.data(), fds.size(), 2);
				// std::cout <<4 << std::endl;
				// if (errno != 0 && errno != 2)
				// {
				// 	perror("aa1111");
				// 	sleep(3);
				// }
				write_level(servers, fds);
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
