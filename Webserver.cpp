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

// about errcode, could be 404, forbidden, no permision, bla bla, later.
void	handle_unknown_request(int &new_socket, std::string &path)
{
	(void) path;
	std::cout << "handle_unkown_request called" << std::endl;
	char http_404[1024] = "HTTP/1.1 404 KO\r\n"
                                        "Content-Type: text/html\r\n"
                                        "\r\n"
										"<html><body>404-File Not Found</body></html>";
	if (write(new_socket, http_404, strlen(http_404)) < 0)
		throw std::runtime_error("write Failed");
}

void	handle_post_request(int	&new_socket, std::string &request_content, std::string &path)
{
	std::cout << "handle_post_request called" << std::endl;
	(void) new_socket;
	(void) path;
	ssize_t pos = request_content.find("data=");
	if (pos == std::string::npos)
		handle_unknown_request(new_socket, path);
	else
	{
		std::string data = request_content.substr(pos + 5, request_content.end() - request_content.begin());
		std::ofstream file("data.txt", std::ios::app);
		file << data << std::endl;
		file.close();
	}	
}


void	handle_delete_request(int &new_socket, std::string &path)
{
	std::cout << "handle_delete	_request called" << std::endl;
	(void)new_socket;
	(void)path;
	std::string command = "rm " + path;
	int	result = std::system(command.c_str());
	if (result == 0)
		std::cout << "Success" << std::endl;
	else
		std::cout << "Removed failed" << std::endl;
}

void	handle_get_request(int &new_socket, std::string &path)
{
	std::cout << "handle_get_request called" << std::endl;
	struct stat 	info;
	char			buffer[BUFFER_SIZE + 1];
	int				infile_fd;
	std::string		http_header = "HTTP/1.1 200 OK\r\n"
                                        "Content-Type: text/html\r\n"
                                        "\r\n";

	std::cout << "path = " << path.c_str() << std::endl;
	infile_fd = open(path.c_str(), O_RDONLY);
	if (stat(path.c_str(), &info) != 0)
	{
		perror("stat: ");
		// std::cout << 1 << std::endl;
		handle_unknown_request(new_socket, path);
	}
	else if (infile_fd == -1)
	{
		// std::cout << 2 << std::endl;
		handle_unknown_request(new_socket, path);
	}
	else if (!S_ISREG(info.st_mode) && !S_ISLNK(info.st_mode))
	{
		// std::cout << 3 << std::endl;	
		handle_unknown_request(new_socket, path);
	}
	else
	{
		// std::cout << "get in" << std::endl;
		http_header += read_file(infile_fd);
		if (write(new_socket, http_header.c_str(), http_header.length()) < 1)
			throw std::runtime_error("write Failed");
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
