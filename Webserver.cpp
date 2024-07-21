#include "Tomweb.hpp"
#include <algorithm>
#include <vector>


// void set_non_blocking(int socket) {
//     int flags = fcntl(socket, F_GETFL, 0);
//     if (flags == -1) {
//         perror("fcntl F_GETFL");
//         exit(EXIT_FAILURE);
//     }
//     if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
//         perror("fcntl F_SETFL");
//         exit(EXIT_FAILURE);
//     }
// }

int	main(int ac, char **av)
{
	t_server_data				server_data;
	int							new_socket;
	size_t						addrlen = sizeof(server_data.server_addr);
	std::string					method;
	std::string					path;
	std::string					request_content;
	fd_set						read_fds;
	fd_set						except_fds;
	std::vector<int>			client_socket;
	std::vector<int>::iterator	it;
	int							max_fd;
	int							activity;
	int							cnt;

	FD_ZERO(&read_fds);
	FD_ZERO(&except_fds);

	try {
		load_config(ac, av, server_data.config);
		server_data.server_fd = socket_create(server_data.config, server_data.server_addr);
		client_socket.push_back(server_data.server_fd);
		// set_non_blocking(server_data.server_fd);
		while (1)
		{
			std::cout << "///" << std::endl;
			FD_ZERO(&read_fds);
			max_fd = server_data.server_fd;
			for (std::vector<int>::iterator it = client_socket.begin(); it != client_socket.end(); it ++)
			{
				FD_SET(*it, &read_fds);
				std::cout << "FD_SET {" << *it << "}"<< std::endl;
				if (max_fd < *it)
					max_fd = *it;
			}

			activity = select(max_fd + 1, &read_fds, NULL, &except_fds, NULL);
			std::cout << "activiry = " << activity << std::endl;
			std::cout << "select called" << std::endl;
			if (activity < 0  && errno != EINTR)
			{
				perror("select error");
				sleep(1);
			}
			else
			{
				cnt = -1;
				while (1)
				{
					cnt++;
					it = client_socket.begin() + cnt;
					if (it == client_socket.end())
						break ;
					std::cout << "*it = " << *it << std::endl;
					if (FD_ISSET(*it, &read_fds))
					{
						if (*it == server_data.server_fd)
						{
							if ((new_socket = accept(server_data.server_fd,
								(struct sockaddr *)&server_data.server_addr, (socklen_t*)&addrlen)) < 0)
								return (perror("accept failed"), close(server_data.server_fd), 1);
							client_socket.push_back(new_socket);
							// set_non_blocking(new_socket);
							std::cout << *((client_socket).end() - 1)  << "is added"<< std::endl;
						}
						else
						{
							std::cout << "descriptor " << *it << " is called here" << std::endl;
							if (method_find(*it, method, path, request_content) == 1)
							{
								std::cout << "Method = " << method << std::endl;
								if (method == "POST")
									handle_post_request(new_socket, request_content, path);
								if (method == "GET")
									handle_get_request(new_socket, path);
								else if (method == "DELETE")
									handle_delete_request(new_socket, path);
								else
									handle_unknown_request(new_socket, path);
								std::cout << "ends" << std::endl;
							}
							// close(*it);
							// client_socket.erase(it);
							// cnt--;
						}
					}
				}
			}
		}
	}
	catch (const std::runtime_error&e) {
		close(server_data.server_fd);
		perror(e.what());
	}
	return (1);
}
