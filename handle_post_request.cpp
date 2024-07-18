#include "Tomweb.hpp"

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
