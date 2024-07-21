#include "webserv.hpp"

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

int	main(int ac, char **av)
{
	std::vector<server_t> s;
	if (parse("./Configs/default.conf", s))
	{
		std::cout << "Error " << std::endl;
	}
	
	Server 				serv(s[0]);
	int					new_socket;
	size_t				addrlen = sizeof(sockaddr_in);
	std::string			method;
	std::string			path;
	std::string			request_content;


	try {
		fd_set master_set, read_fds;
    	int fd_max;
		FD_ZERO(&master_set);
		FD_ZERO(&read_fds);
		socklen_t sin_size;
		FD_SET(serv.serverFd, &master_set);
		fd_max = serv.serverFd;
		int new_fd;
		struct sockaddr_in address;
		while (1)
		{
			// Copy master_set to read_fds before calling select
			read_fds = master_set;
			std::cout << "bla bla" << std::endl;
			if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1) {
				perror("select");
				exit(EXIT_FAILURE);
			}
			std::cout << "asdasd" << std::endl;
			// Handle connections	
			for (int i = 0; i <= fd_max; i++) {
				if (FD_ISSET(i, &read_fds)) {
					if (i == serv.serverFd) {
						// Handle new connections
						sin_size = sizeof(address);
						new_fd = accept(i, (struct sockaddr *)&address, &sin_size);
						if (new_fd == -1) {
							perror("accept");
						} else {
							// Print client IP and port
							char client_ip[INET_ADDRSTRLEN];
							inet_ntop(AF_INET, &address.sin_addr, client_ip, sizeof(client_ip));
							printf("New connection from %s:%d on socket %d\n",
								client_ip, ntohs(address.sin_port), new_fd);

							// Add new socket to the master set
							FD_SET(new_fd, &master_set);
							if (new_fd > fd_max) {
								fd_max = new_fd;
							}
						}
					} else {
						// Handle data from a client
						// handle_client(i);
						FD_CLR(i, &master_set);
					}
				}
			}
		}


	}
	catch (const std::runtime_error&e) {
		close(serv.serverFd);
		perror(e.what());
	}
	
	return (0);
}
