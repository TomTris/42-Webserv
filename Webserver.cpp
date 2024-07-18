#include "Tomweb.hpp"

std::string read_file(int fd)
{
	std::string	ret;
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE + 1];

	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			throw	std::runtime_error("read Failed");
		if (bytes_read == 0)
			return (ret);
		buffer[bytes_read] = 0;
		ret += std::string(buffer);
	}
	return (ret);
}

void	handle_post_request(int	&new_socket, std::string &path)
{
	std::cout << "handle_post_request called" << std::endl;
	(void) new_socket;
	(void) path;
}

void	handle_unknown_request(int &new_socket)
{
	(void) new_socket;
	std::cout << "handle_unkown_request called" << std::endl;
}

void	handle_delete_request(int &new_socket, std::string &path)
{
	(void)new_socket;
	(void)path;
}

void	handle_get_request(int &new_socket, std::string &path)
{
	std::cout << "handle_get_request called" << std::endl;
	std::ifstream 	file(path);
	char			buffer[BUFFER_SIZE + 1];
	std::string		infile;
	std::string http_header_200 = "HTTP/1.1 200 OK\r\n"
									"Content-Type: text/html\r\n"
									"\r\n";

	if (!file)
		handle_unkown_request(new_socket);
	else
	{
		infile = read_file(new_socket);
		if (send(new_socket, http_header_200.c_str(), http_header_200.length(), 0) < 0
			|| send(new_socket, infile.c_str(), infile.length(), 0) < 0)
			throw std::runtime_error("send Failed");
	}
}

int	main(int ac, char **av)
{
	t_server_data		server_data;
	int					new_socket;
	size_t				addrlen = sizeof(server_data.server_addr);
	std::string			method;
	std::string			path;
	std::string			request_content;

	try {
		load_config(ac, av, server_data.config);
		server_data.server_fd = socket_create(server_data.config, server_data.server_addr);
		while (1)
		{
			if ((new_socket = accept(server_data.server_fd,
				(struct sockaddr *)&server_data.server_addr, (socklen_t*)&addrlen)) < 0)
				return (perror("accept failed"), close(server_data.server_fd), 1);
			method_find(new_socket, method, path, request_content);
			if (method == "POST")
				handle_post_request(new_socket, path);
			else if (method == "GET")
				handle_get_request(new_socket, path);
			else if (method == "DELETE")
				handle_delete_request(new_socket, path);
			else
				handle_unknown_request(new_socket);
			close(new_socket);
		}
	}
	catch (const std::runtime_error&e) {
		close(server_data.server_fd);
		std::cerr << e.what() << std::endl;
	}
	return (1);
}
