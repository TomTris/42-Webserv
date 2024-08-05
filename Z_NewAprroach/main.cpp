#include "Tomweb.hpp"

std::vector<struct pollfd>  fds;
unsigned int Server::cookies_nbr;

int	directory_exists(const char *path)
{
	struct stat info;

	if (stat(path, &info) == 0)
	{
		if (S_ISDIR(info.st_mode))
			return (1);
	}
	return (0);
}

void	prepare(void)
{
	int	fd;
	
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	if (BUFFERSIZE > 10000)
		throw std::runtime_error( "Too big BUFFERSIZE" );
	if (BUFFERSIZE < 2)
		throw std::runtime_error( "Invalid BUFFERSIZE" );
	Server::cookies_nbr = 0;

	if (directory_exists("./cgi-bin/") == 0)
		throw std::runtime_error("./cgi-bin/ isn't a directory");
	fd = open("./cgi-bin/cookies", O_CREAT | O_TRUNC | O_RDWR, 0777);
	if (fd == -1)
		throw std::runtime_error("Couldn't empty ./cgi-bin/cookies");
	close(fd);
}

int	main(int ac, char **av)
{
	std::vector<Server>			servers;

	try
	{
		prepare();
		load_config_n_socket_create(ac, av, servers);
		add_servers_to_pool(servers);
		std::cout << "ok " << std::endl;
		while(1)
		{
			poll(fds.data(), fds.size(), 0);
			
			server_level(servers);
			connection_level(servers);
			read_level(servers);
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
