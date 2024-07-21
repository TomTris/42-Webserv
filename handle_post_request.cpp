#include "Tomweb.hpp"

void	boundary_find()

void	handle_post_request(int	&new_socket, std::string &request_content, std::string &path)
{
	(void) new_socket;
	if (path == "upload")
	{
		int	a = open("123", O_RDWR);
		ssize_t header_skip = request_content.find("\r\n\r\n");
		std::string body = request_content.substr(header_skip + 4);
		write(a, body.c_str(), body.length());

	}
	else if (path == "text_form")
	{
		std::cerr << "request text_form called" << std::endl;
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
	else
		std::cerr << "non post is called" << std::endl;
}
