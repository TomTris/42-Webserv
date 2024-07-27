#include "Tomweb.hpp"

int	main(int ac, char **av)
{
	std::vector<Server>			servers;
	fd_set						read_fds;
	fd_set						write_fds;
	struct timeval				tv;
	std::vector<int> 			to_del_fds;
	int							max_fd;
	int							activity;
	int							re_try;

	re_try = 1000;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	try {
		FD_ZERO(&read_fds);
		load_config_n_socket_create(ac, av, servers, max_fd);
	}
	catch (const std::runtime_error&e) {
		std::cout << "throw called"<<std::endl;
		perror(e.what());
		return (0);
	}
	std::cout << "hhmm"<<std::endl;
	return (0);
}
