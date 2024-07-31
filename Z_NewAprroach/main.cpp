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
		std::cout << "ok " << std::endl;
		while(1)
		{
			activity = poll(fds.data(), fds.size(), 0);
			if (activity < 0)
			{
				perror("select error");
				if (--re_try == 0)
					return (0);
			}
			else
			{
				server_level(servers, fds);
				poll(fds.data(), fds.size(), 0);

				connection_level(servers, fds);
				// std::cout << 1 << std::endl;
				check_fds(fds, 0);
				usleep(10000);
				std::cout << 1 << std::endl;
				// {
				// 	std::cout << 11 << std::endl;
				// 	std::cout << "here have read  after2 = {" << servers[0].connections[0].have_read << "}\n--------------------------------------------------------\n\n"<< std::endl;
				// }
				// std::cout << 1 << std::endl;
				poll(fds.data(), fds.size(), 0);
				// sleep(1);
				check_fds(fds, 0);
				read_level(servers, fds);
				// if (check_fds(fds, 4) == POLLIN || check_fds(fds, 4) == POLLOUT)
				// {
				// 	std::cout << 2222<< std::endl;
				// 	std::cout << "here have read  after2 = {" << servers[0].connections[0].have_read << "}\n--------------------------------------------------------\n\n"<< std::endl;
				// }
				poll(fds.data(), fds.size(), 0);

				write_level(servers, fds);
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
