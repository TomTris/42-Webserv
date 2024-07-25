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

std::string	get_status_code(int nbr)
{
	if (nbr == 200)
		return handle_200();
	if (nbr == 204)
		return handle_204();
	if (nbr == 400)
		return handle_400();
	if (nbr == 401)
		return handle_401();
	if (nbr == 403)
		return handle_403();
	if (nbr == 404)
		return handle_404();
	if (nbr == 408)
		return handle_408();
	if (nbr == 409)
		return handle_409();
	if (nbr == 411)
		return handle_411();
	std::cerr << " ? ?? ?? get_status code" << std::endl;
	return ("");
}

void	load_config_n_socket_create(int ac, char **av, std::vector<Server> &servers, int &max_fd)
{
	std::vector<server_t>	server_config;
	load_config(ac, av, server_config);
	max_fd = 0;
	for (int i = 0; i < server_config.size(); i++)
	{
		servers.push_back(Server(server_config[i]));
		if (servers[i].serverFd > max_fd)
			max_fd = servers[i].serverFd;
	}
	if (servers.size() == 0)
		throw std::runtime_error("No Server");
}