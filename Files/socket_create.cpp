#include "../Tomweb.hpp"

void	load_config(int ac, char **av, std::vector<server_t> &server_config)
{
	if (ac != 1 && ac != 2)
		throw std::runtime_error("Too many config files");
	if (ac == 2)
		parse(av[1], server_config);
	else
		parse("Configs/default.conf", server_config);
}

void	load_config_n_socket_create(int ac, char **av, std::vector<Server> &servers)
{
	std::vector<server_t>	server_config;
	load_config(ac, av, server_config);
	for (int i = 0; i < server_config.size(); i++)
	{
		servers.push_back(Server(server_config[i]));
	}
}