#include "Tomweb.hpp"

// about errcode, could be 404, forbidden, no permision, bla bla, later.
void	handle_404(int &new_socket)
{
	char http_404[1024] = "HTTP/1.1 404 KO\r\n"
                                        "Content-Type: text/html\r\n"
                                        "\r\n"
										"<html><body>404-File Not Found</body></html>";
	if (write(new_socket, http_404, strlen(http_404)) < 0)
		throw std::runtime_error("write Failed");
}

void	handle_unknown_request(int &new_socket, std::string &path)
{
	(void) path;
	std::cout << "handle_unkown_request called" << std::endl;
	handle_404(new_socket);
}
