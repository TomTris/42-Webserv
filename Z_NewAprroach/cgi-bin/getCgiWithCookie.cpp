#include "../Tomweb.hpp"

int	main()
{
	std::string ret =
	"Content-Type: text/html\r\n\r\n"
	"<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
	"	<title>CGI Test</title>\n"
	"</head>\n"
	"<body>\n"
	"	<p> Your Cookies is:" + (std::string) getenv("HTTP_COOKIE") + "</p>\n"
	"</body>\n"
	"</html>\n";
	std::cout << ret << std::endl;
	return (0);
}
