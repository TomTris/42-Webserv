#include "Tomweb.hpp"

// void before()
// {
// 	sockaddr_in address;
// 	method = "";
// 	path = "" ;
// 	request_content = "";
// 	if ((new_socket = accept(serv.serverFd,
// 		(struct sockaddr *)&serv.address, (socklen_t*)&addrlen)) < 0)
// 		return (perror("accept failed"), close(serv.serverFd), 1);
// 	if (method_find(new_socket, method, path, request_content) == 1)
// 	{
// 		std::cout << "Method = " << method << std::endl;
// 		// if (method == "POST")
// 		// 	handle_post_request(new_socket, request_content, path);
// 		// else if (method == "GET")
// 		// 	handle_get_request(serv, new_socket, path);
// 		// else if (method == "DELETE")
// 		// 	handle_delete_request(new_socket, path);
// 		// else
// 		// 	handle_unknown_request(new_socket, path);
// 		location loc = get_location(serv.locations, path);
// 		if (loc.doesntExist)
// 		{
// 			path = get_error_page(serv, 404);
// 			handle_get_request(new_socket, path);
// 		}
// 		else
// 		{
// 			std::vector<std::string> allowedMethods = get_allowed(loc);
// 			if (method == "POST")
// 			{
// 				if (std::find(allowedMethods.begin(), allowedMethods.end(), method) != allowedMethods.end())
// 				{
// 					path = get_path_to_file(loc, path);
// 					if (isDirectory(path.c_str()))
// 					{
// 						//toDo ls similar stuff if needed otherwice error
// 					}
// 					else
// 					{
// 						if (access(path.c_str(), F_OK) == 0)
// 							handle_post_request(new_socket, request_content, path);
// 						else
// 						{
// 							path = get_error_page(serv, 404);
// 							handle_get_request(new_socket, path);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					path = get_error_page(serv, 405);
// 					handle_get_request(new_socket, path);
// 				}
// 			}
// 			else if (method == "GET")
// 			{
// 				if (std::find(allowedMethods.begin(), allowedMethods.end(), method) != allowedMethods.end())
// 				{
// 					path = get_path_to_file(loc, path);
// 					if (isDirectory(path.c_str()))
// 					{
// 						//toDo ls similar stuff if needed otherwice error
// 					}
// 					else
// 					{
// 						if (access(path.c_str(), F_OK) == 0)
// 							handle_get_request(new_socket, path);
// 						else
// 						{
// 							path = get_error_page(serv, 404);
// 							handle_get_request(new_socket, path);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					path = get_error_page(serv, 405);
// 					handle_get_request(new_socket, path);
// 				}
// 			}
// 			else if (method == "DELETE")
// 			{
// 				if (std::find(allowedMethods.begin(), allowedMethods.end(), method) != allowedMethods.end())
// 				{
// 					path = get_path_to_file(loc, path);
// 					if (isDirectory(path.c_str()))
// 					{
// 						//toDo ls similar stuff if needed otherwice error
// 					}
// 					else
// 					{
// 						if (access(path.c_str(), F_OK) == 0)
// 							handle_delete_request(new_socket, path);
// 						else
// 						{
// 							path = get_error_page(serv, 404);
// 							handle_get_request(new_socket, path);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					path = get_error_page(serv, 405);
// 					handle_get_request(new_socket, path);
// 				}
// 			}
// 			else
// 			{
// 				path = get_error_page(serv, 405);
// 				handle_get_request(new_socket, path);
// 			}
// 		}
// 	}
// 	close(new_socket);
// }

void	handle_event(std::vector <int> &client_socket, fd_set &read_fds, t_server_data &server_data)
{	
	size_t						addrlen = sizeof(server_data.server_addr);
	std::string					method;
	std::string					path;
	std::string					request_content;
	int							cnt;
	int							new_socket;
	std::vector<int>::iterator	it;

	cnt = -1;
	while (1)
	{
		cnt++;
		it = client_socket.begin() + cnt;
		if (it == client_socket.end())
			break ;
		if (FD_ISSET(*it, &read_fds))
		{
			if (*it == server_data.server_fd)
			{
				if ((new_socket = accept(server_data.server_fd, (struct sockaddr *)&server_data.server_addr, (socklen_t*)&addrlen)) < 0)
					throw std::runtime_error("accept failed");
				client_socket.push_back(new_socket);
			}
			else
			{
				if (method_find(*it, method, path, request_content) == 1)
				{
					std::cerr << "method = " << method << std::endl;
					if (method == "POST")
						handle_post_request(*it, request_content, path);
					else if (method == "GET")
						handle_get_request(*it, path);
					else if (method == "DELETE")
						handle_delete_request(*it, path);
					else
						handle_unknown_request(*it, path);
				}
				close(*it);
				client_socket.erase(it);
				cnt--;
			}
		}
	}
}


void	set_init(fd_set &read_fds, t_server_data &server_data, int &max_fd, std::vector<int> &client_socket) {
	FD_ZERO(&read_fds);
	max_fd = server_data.server_fd;
	for (std::vector<int>::iterator it = client_socket.begin(); it != client_socket.end(); it ++)
	{
		FD_SET(*it, &read_fds);
		if (max_fd < *it)
			max_fd = *it;
	}
}

int	main(int ac, char **av)
{
	t_server_data				server_data;
	fd_set						read_fds;
	std::vector<int>			client_socket;
	int							max_fd;
	int							activity;


	try {
		FD_ZERO(&read_fds);
		load_config(ac, av, server_data.config);
		server_data.server_fd = socket_create(server_data.config, server_data.server_addr);
		client_socket.push_back(server_data.server_fd);
		while (1)
		{
			set_init(read_fds, server_data, max_fd, client_socket);
			activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
			if (activity < 0  && errno != EINTR)
			{
				perror("select error");
				sleep(1);
			}
			else
			{
				handle_event(client_socket, read_fds, server_data);
			}
		}


	}
	catch (const std::runtime_error&e) {
		close(server_data.server_fd);
		perror(e.what());
	}
	
	return (0);
}
