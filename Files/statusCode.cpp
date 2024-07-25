#include "../Tomweb.hpp"

std::string handle_200(void)
{
	std::string http = "HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"\r\n"
				"<html><body>"
				"OK"
				"</body></html>";
	return (http);
}

//Request proccessed successfull
std::string	handle_204(void)
{
	std::string http = 	"HTTP/1.1 204 KO\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"<html><body>"
			"Request proccessed successful\n"
			"</body></html>";
	return (http);
}

std::string	handle_400(void)
{
	std::string http = "HTTP/1.1 400 Bad Request\r\n"
			"Content-Type: text/html\r\n\r\n<html><body>"
			"Bad Request\n"
			"</body></html>";
	return (http);
}

//Unauthorized
std::string	handle_401(void)
{
	std::string http = "HTTP/1.1 401 Unauthorized\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"<html><body>"
				"Permission denied\n"
			"</body></html>";
	return (http);
}

//Forbidden
std::string	handle_403(void)
{
	std::string http = "HTTP/1.1 403 KO\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"<html><body>"
				"Forbidden\n"
			"</body></html>";
	return (http);
}

//Not found
std::string	handle_404(void)
{
	std::string http ="HTTP/1.1 404 Not found\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"<html><body>"
				"404 Not Found"
			"</body></html>";
	return (http);
}

//Request Timeout
std::string	handle_408(void)
{
	std::string http = "HTTP/1.1 408 Timeout\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"<html><body>"
				"Request Timeout\n"
			"</body></html>";
	return (http);
}

//Conflict
std::string	handle_409(void)
{
	std::string http = "HTTP/1.1 409 Conflict\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"<html><body>"
				"Conflict\n"
			"</body></html>";
	return (http);
}

//Content-Length required
std::string	handle_411(void)
{
	std::string http = "HTTP/1.1 411 Content-Length required\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"<html><body>"
			"Content-Length required\n"
			"</body></html>";
	return (http);
}

