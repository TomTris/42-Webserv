#include "Tomweb.hpp"

std::string read_socket(int fd)
{
	std::string	ret;
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE + 1];

	while (1)
	{
		bytes_read = recv(fd, buffer, BUFFER_SIZE, 0);
		if (bytes_read == -1)
			throw	std::runtime_error("recv Failed");
		if (bytes_read == 0)
			return (ret);
		buffer[bytes_read] = 0;
		ret += std::string(buffer);
	}
	throw std::runtime_error("empty request?");
}

void	method_find(int	new_socket, std::string &method, std::string &path, std::string &request_content)
{
	std::string HTTP_version;
	std::string request_content = read_socket(new_socket);
	std::stringstream socket_stream(request_content);

	if (!(socket_stream >> method >> path >> HTTP_version))
	{
		std::cerr << request_content << std::endl;
		throw std::runtime_error("weird request!");
	}
}
