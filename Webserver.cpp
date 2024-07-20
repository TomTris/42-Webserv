#include "webserv.hpp"



int	main(int ac, char **av)
{
	std::vector<server_t> s;
	if (parse("./Configs/default.conf", s))
	{
		std::cout << "Asdasfas" << std::endl;
	}
	
	Server 				serv(s[0]);
	int					new_socket;
	size_t				addrlen = sizeof(sockaddr_in);
	std::string			method;
	std::string			path;
	std::string			request_content;


	try {
		
		while (1)
		{
			sockaddr_in address;
			method = "";
			path = "" ;
			request_content = "";
			if ((new_socket = accept(serv.serverFd,
				(struct sockaddr *)&serv.address, (socklen_t*)&addrlen)) < 0)
				return (perror("accept failed"), close(serv.serverFd), 1);
			if (method_find(new_socket, method, path, request_content) == 1)
			{
				std::cout << "Method = " << method << std::endl;
				// if (method == "POST")
				// 	handle_post_request(new_socket, request_content, path);
				// else if (method == "GET")
				// 	handle_get_request(serv, new_socket, path);
				// else if (method == "DELETE")
				// 	handle_delete_request(new_socket, path);
				// else
				// 	handle_unknown_request(new_socket, path);
				location loc = get_location(serv.locations, path);
			}
			close(new_socket);
		}
	}
	catch (const std::runtime_error&e) {
		close(serv.serverFd);
		perror(e.what());
	}
	
	return (0);
}
