#include "Tomweb.hpp"

#define BUFFER_SIZE 1024

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
	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int	opt = 1;
	if (server_fd == -1)
		return (perror("socket"), -1);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(config.port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	//need to learn more about htis
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
		return (perror("setsockopt failed"), close(server_fd), -1);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		return (perror("bind"), close(server_fd), -1);
	if (listen (server_fd, config.listen_max) == -1)
		return (perror("listen"), close(server_fd), -1);
	return (server_fd);
}

std::string read_socket(int fd)
{
	std::string	ret;
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE + 1];

	while (1)
	{
		bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);
		if (bytes_read == -1)
			return (perror("recv"), "");
		if (bytes_read == 0)
			return (ret);
		buffer[bytes_read] = 0;
		ret += std::string(buffer);
	}
	return ("");
}

std::string read_file(int fd)
{
	std::string	ret;
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE + 1];

	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (perror("read"), "");
		if (bytes_read == 0)
			return (ret);
		buffer[bytes_read] = 0;
		ret += std::string(buffer);
	}
	return ("");
}

std::string	method_find(int	new_socket, t_server_data server_data)
{
	std::string 
}

int	main(int ac, char **av)
{
	t_server_data	server_data;
	int					new_socket;
	size_t				addrlen = sizeof(server_data.server_addr);
	std::string			method;

	load_config(ac, av, server_data.config);
	server_data.server_fd = socket_create(server_data.config, server_data.server_addr);
	if (server_data.server_fd == -1)
		return (1);
	while (1)
	{
		if ((new_socket = accept(server_data.server_fd,
			(struct sockaddr *)&server_data.server_addr, (socklen_t*)&addrlen)) < 0)
            return (perror("accept failed"), close(server_data.server_fd), 1);

		method = method_find(new_socket, server_data);
		if (method == "POST")
			handle_post_request(new_socket, server_data);
		else if (method == "GET")
			handle_get_request(new_socket, server_data);
		else if (method == "DELETE")
			handle_delete_request(new_socket, server_data);
		else
			handle_unknown_request(new_socket, server_data);
		close(new_socket);
	}
	return (1);
}