#include "Tomweb.hpp"

int	main(int ac, char **av, char **env)
{
	std::vector<Server>			servers;
	std::vector<struct pollfd> fds;
	int							activity;
	int							re_try;
	int	a;
	re_try = 1000;
	if (BUFFERSIZE > 10000)
	{
		std::cerr << "Too big BUFFERSIZE" << std::endl;
		return (1);
	}
	if (BUFFERSIZE < 2)
	{
		std::cerr << "Invalid BUFFERSIZE" << std::endl;
		return (1);
	}
	try {
		load_config_n_socket_create(ac, av, servers);
		add_servers_to_pool(servers, fds);
		while(1)
		{
			activity = poll(fds.data(), fds.size(), 0);
			// std::cout << "fds.size = " << fds.size() << std::endl;
			// for (int i = 0; i < fds.size(); i++)
			// {
			// 	std::cout << fds[i].fd << ", " << std::ends;
			// }
			// std::cout << "\nwith options" << std::ends;
			// for (int i = 0; i < fds.size(); i++)
			// {
			// 	std::cout << fds[i].events << ", " << std::ends;
			// }
			// std::cout << "\n with revents" << std::ends;
			// for (int i = 0; i < fds.size(); i++)
			// {
			// 	std::cout << fds[i].revents << ", " << std::ends;
			// }
			// usleep(2000);
			// check_fds(fds, fd);
			if (activity < 0)
			{
				perror("select error");
				if (--re_try == 0)
					return (0);
			}
			else
			{
				// usleep(500000);
				server_level(servers, fds);
			poll(fds.data(), fds.size(), 0);
				connection_level(servers, fds);
			poll(fds.data(), fds.size(), 0);
				// std::cout << "\n2. POLL-----" << std::endl;
				// std::cout << "fds.size = " << fds.size() << std::endl;
				// for (int i = 0; i < fds.size(); i++)
				// {
				// 	std::cout << fds[i].fd << ", " << std::ends;
				// }
				// std::cout << "\nwith options" << std::ends;
				// for (int i = 0; i < fds.size(); i++)
				// {
				// 	std::cout << fds[i].events << ", " << std::ends;
				// }
				// std::cout << "\n with revents" << std::ends;
				// for (int i = 0; i < fds.size(); i++)
				// {
				// 	std::cout << fds[i].revents << ", " << std::ends;
				// }
				// std::cout << "---2---" << std::endl;
				read_level(servers, fds);
			poll(fds.data(), fds.size(), 0);
			sleep(1);
				// std::cout << "----3--" << std::endl;
				write_level(servers, fds);
				// std::cout << "--" << a++ << "----" << std::endl;
			}
		// usleep(250000);
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
