#include "webserv.hpp"



void	handle_get_request(int &new_socket, std::string &path)
{
	std::cout << "handle_get_request called" << std::endl;
	struct stat 	info;
	char			buffer[BUFFER_SIZE + 1];
	int				infile_fd;
	std::string		http_header = "HTTP/1.1 200 OK\r\n"
                                        "Content-Type: text/html\r\n"
                                        "\r\n";
	std::cout << path << std::endl;
	
	infile_fd = open(path.c_str(), O_RDONLY);
	std::cout << "path = " << path.c_str() << std::endl;
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
