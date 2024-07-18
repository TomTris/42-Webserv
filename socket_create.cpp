#include "Tomweb.hpp"

void	load_config(int ac, char **av, t_server_config &config)
{
	(void) ac;
	(void) av;
	config.port = 8080;
	config.root = "./";
	config.listen_max = 3;
}

int	socket_create(t_server_config &config, struct sockaddr_in &server_addr)
{
	int	opt = 1;
	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		throw	std::runtime_error("socket Failed");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(config.port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	//need to learn more about htis
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
		throw	std::runtime_error("setsockopt Failed");
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		throw	std::runtime_error("bind Failed");
	if (listen (server_fd, config.listen_max) == -1)
		throw	std::runtime_error("listen Failed");
	return (server_fd);
}
