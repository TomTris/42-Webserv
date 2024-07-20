#include "webserv.hpp"

void read_socket(int &new_socket, std::string &request_content)
{
	ssize_t	bytes_read = 4096;
	char	buffer[4096 + 1];

	while (1)
	{
		if (bytes_read < 4096)
			return ;
		bytes_read = read(new_socket, buffer, 4096);
		if (bytes_read == -1)
			throw	std::runtime_error("read Failed");
		buffer[bytes_read] = 0;
		request_content += std::string(buffer);
	}
}

int	method_find(int	&new_socket, std::string &method, std::string &path, std::string &request_content)
{
	std::string HTTP_version;

	read_socket(new_socket, request_content);
	std::stringstream socket_stream(request_content);

	if (!(socket_stream >> method >> path >> HTTP_version))
	{
		return (0);
	}
	
	// std::string::iterator ite = path.begin();
	// while (*ite == '/')
	// {
	// 	path.erase(path.begin());
	// 	ite = path.begin();
	// }
	return (1);
}
