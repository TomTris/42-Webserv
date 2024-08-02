#include "Tomweb.hpp"

void	load_config(int ac, char **av, std::vector<server_t> &server_config)
{
	if (ac != 1 && ac != 2)
		throw std::runtime_error("Too many config files");
	if (ac == 2)
		parse(av[1], server_config);
	else
		parse("Configs/default.conf", server_config);
}

int check_if_exists(int host, int port, std::vector<Server> &servers)
{
	for (unsigned int i = 0; i < servers.size(); i++)
	{
		if (servers[i].port == port && servers[i].host == host)
		{
			return (i);
		}
	}
	return (-1);
}

void	load_config_n_socket_create(int ac, char **av, std::vector<Server> &servers)
{
	std::vector<server_t>	server_config;
	load_config(ac, av, server_config);

	for (unsigned int i = 0; i < server_config.size(); i++)
	{
		int ans = check_if_exists(server_config[i].host, server_config[i].port, servers);
		if (ans == -1)
		{
			servers.push_back(Server(server_config[i]));
			if (servers.back().err == 1)
				throw std::runtime_error("failed while creating socket");
		}
		else
		{
			std::string server_name = server_config[i].servername;
			if (find(servers[ans].server_names.begin(), servers[ans].server_names.end(), server_name) != servers[ans].server_names.end())
				throw std::runtime_error("Two servers cant have same ports and server_names");
			if (server_name == "")
			{
				servers[ans].locations.insert(servers[ans].locations.begin(), server_config[i].locations);
				servers[ans].server_names.insert(servers[ans].server_names.begin(), server_name);
			}
			else
			{
				servers[ans].locations.push_back(server_config[i].locations);
				servers[ans].server_names.push_back(server_name);
			}
		}
	}
	for (unsigned int i = 0; i < servers.size(); i++)
	{
		if (servers[i].server_names[0] != "")
			servers[i].server_names[0] = "";
	}
	if (servers.size() == 0)
		throw std::runtime_error("No Server");
}
