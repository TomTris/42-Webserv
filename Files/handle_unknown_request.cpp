#include "../Tomweb.hpp"

std::string	header_200(void)
{
	std::string ret;
	
	ret = "HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"\r\n";
	return (ret);
}

void handle_200(int &new_socket, std::string &str)
{
	std::string ret;
	
	ret = "HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"\r\n<html><body>";
	ret += str;
	ret += "</body></html>";
	if (write(new_socket, ret.c_str(), ret.length()) < 0)
		throw std::runtime_error("write Failed");
}

//Request proccessed successfull
void	handle_204(int &new_socket)
{
	std::string http = "HTTP/1.1 204 KO\r\nContent-Type: text/html\r\n\r\n<html><body>";
	std::string http2 = "Request proccessed successful\n";
	std::string http3 = "</body></html>";
	http = http + http2 + http3;
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

//Bad request
void	handle_400(int &new_socket, std::string const &str)
{
	std::string http = "HTTP/1.1 400 KO\r\nContent-Type: text/html\r\n\r\n<html><body>";
	std::string http2 = "Bad Request\n";
	std::string http3 = "</body></html>";
	http = http + http2 + str + http3;
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

//Unauthorized
void	handle_401(int &new_socket)
{
	std::string http = "HTTP/1.1 401 KO\r\nContent-Type: text/html\r\n\r\n<html><body>";
	std::string http2 = "Permission denied\n";
	std::string http3 = "</body></html>";
	http = http + http2 + http3;
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

//Forbidden
void	handle_403(int &new_socket)
{
	std::string http = "HTTP/1.1 403 KO\r\nContent-Type: text/html\r\n\r\n<html><body>";
	std::string http2 = "Forbidden\n";
	std::string http3 = "</body></html>";
	http = http + http2 + http3;
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

//Not found
void	handle_404(int &new_socket)
{
	char http_404[1024] = "HTTP/1.1 404 KO\r\n"
                                        "Content-Type: text/html\r\n"
                                        "\r\n"
										"<html><body>404 Not Found</body></html>";
	if (write(new_socket, http_404, strlen(http_404)) < 0)
		throw std::runtime_error("write Failed");
}

//Request Timeout
void	handle_408(int &new_socket)
{
	std::string http = "HTTP/1.1 408 KO\r\nContent-Type: text/html\r\n\r\n<html><body>";
	std::string http2 = "Request Timeout\n";
	std::string http3 = "</body></html>";
	http = http + http2 + http3;
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

//Conflict
void	handle_409(int &new_socket)
{
	std::string http = "HTTP/1.1 409 KO\r\nContent-Type: text/html\r\n\r\n<html><body>";
	std::string http2 = "Conflict\n";
	std::string http3 = "</body></html>";
	http = http + http2 + http3;
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

//Content-Length required
void	handle_411(int &new_socket)
{
	std::string http = "HTTP/1.1 411 KO\r\nContent-Type: text/html\r\n\r\n<html><body>";
	std::string http2 = "Content-Length required\n";
	std::string http3 = "</body></html>";
	http = http + http2 + http3;
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

void	handle_none(int &new_socket, int nbr, std::string const &str)
{
	std::string http = "HTTP/1.1 ";
	http += nbr;
	http += " KO\r\nContent-Type: text/html\r\n\r\n<html><body>";
	http += str;
	http += "</body></html>";
	if (write(new_socket, http.c_str(), http.length()) < 0)
		throw std::runtime_error("write Failed");
}

void	handle_unknown_request(int &new_socket, std::string &path)
{
	(void) path;
	std::cerr << "handle_unkown_request called" << std::endl;
	handle_404(new_socket);
}
