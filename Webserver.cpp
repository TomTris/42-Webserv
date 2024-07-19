#include "Tomweb.hpp"

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
			std::cout << "Method = " << method << std::endl;
			if (method == "POST")
				handle_post_request(new_socket, request_content, path);
			else if (method == "GET")
				handle_get_request(new_socket, path);
			else if (method == "DELETE")
				handle_delete_request(new_socket, path);
			else
				handle_unknown_request(new_socket, path);
			close(new_socket);
		}
	}
	catch (const std::runtime_error&e) {
		close(server_data.server_fd);
		perror(e.what());
	}
	return (1);
}
