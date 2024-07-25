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

void	get_status_code(Server server)
{
	std::string h200 = handle_200();
	server.statusCode[200] = h200;

	std::string h204 = handle_204();
	server.statusCode[204] = h204;

	std::string h400 = handle_400();
	server.statusCode[400] = h400;

	std::string h401 = handle_401();
	server.statusCode[401] = h401;

	std::string h403 = handle_403();
	server.statusCode[403] = h403;

	std::string h404 = handle_404();
	server.statusCode[404] = h404;

	std::string h408 = handle_408();
	server.statusCode[408] = h408;

	std::string h409 = handle_409();
	server.statusCode[409] = h409;

	std::string h411 = handle_411();
	server.statusCode[411] = h411;
}

void	load_config_n_socket_create(int ac, char **av, std::vector<Server> &servers)
{
	std::vector<server_t>	server_config;
	load_config(ac, av, server_config);
	for (int i = 0; i < server_config.size(); i++)
	{
		servers.push_back(Server(server_config[i]));
	}
	if (servers.size() == 0)
		throw std::runtime_error("No Server");
	get_status_code(servers[0]);
}