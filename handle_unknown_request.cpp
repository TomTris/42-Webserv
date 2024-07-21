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

void	handle_200(int &new_socket, std::string str)
{
	std::string http = "HTTP/1.1 404 KO\r\nContent-Type: text/html\r\n\r\n<html><body>" + str + "</body></html>";
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

void	handle_unknown_request(int &new_socket, std::string &path)
{
	(void) path;
	std::cerr << "handle_unkown_request called" << std::endl;
	handle_404(new_socket);
}
