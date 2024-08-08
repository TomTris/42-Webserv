#include "../Tomweb.hpp"

int	extract_IsAfterResponseClose(std::string &header_o)
{
	std::string	header = header_o;
	std::string	str = "\r\nConnection: ";
	ssize_t	start = header.find(str);
	if (start == static_cast<ssize_t>(std::string::npos))
		return (0);
	start += str.length();
	ssize_t end = header.find("\r\n",  start);
	std::string content = header.substr(start, end - start);
	if (content.find("close") != std::string::npos)
		return (1);
	return (0);
}

int	extract_contentLength(std::string &header_o)
{
	std::string	header = header_o;
	std::string	str = "\r\nContent-Length:";
	ssize_t	start = header.find(str);
	if (start == static_cast<ssize_t>(std::string::npos))
		return (0);
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == static_cast<ssize_t>(std::string::npos))
		return (0);
	std::string content = header.substr(start, end - start);
	std::stringstream socket_stream(content);
	int value;
	socket_stream >> value;
	return (value);
}

std::string	extract_host(std::string &header_o)
{
	std::string	header = header_o;
	std::string	str = "\r\nHost: ";
	ssize_t	start = header.find(str);
	if (start == static_cast<ssize_t>(std::string::npos))
		return ("");
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == static_cast<ssize_t>(std::string::npos))
		return ("");
	std::string content = header.substr(start, end - start);
	std::stringstream socket_stream(content);
	std::string value;
	socket_stream >> value;
	return (value);
}

std::string	extract_cookies(std::string &header)
{
	std::string	str = "\r\nCookie: ";

	ssize_t	start = header.find(str);
	if (start == static_cast<ssize_t>(std::string::npos))
		return ("");
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == static_cast<ssize_t>(std::string::npos))
		return ("");
	std::string ret;
	ret = header.substr(start, end);
	return (ret);
}

int	header_extract(Connection &cnect, std::string &header_o)
{
	std::string host;

	cnect.IsAfterResponseClose = extract_IsAfterResponseClose(header_o);
	cnect.reader.contentLength = extract_contentLength(header_o);
	cnect.reader.contentLengthCGI = cnect.reader.contentLength;
	cnect.reader.host = extract_host(header_o);
	cnect.reader.cookies = extract_cookies(header_o);
	return (1);
}
