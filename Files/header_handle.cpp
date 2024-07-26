#include "../Tomweb.hpp"

// int	extract_contentDisposition(Connection &current_connection, std::string &header_o)
// {
// 	std::string	header = header_o;
// 	std::string	str = "\r\nContent-Disposition:";
// 	ssize_t	start = header.find(str);
// 	if (start != std::string::npos)
// 	{
// 		ssize_t end = header.substr(start + str.length()).find("\r\n");
// 		std::string content = header.substr(start + str.length(), end);
// 		start = content.find("name=\"");
// 		if (start != std::string::npos)
// 		{
// 			start += 6;
// 			end = content.find("\"");
// 			current_connection.form_name = content.substr(start, end);
// 			content.erase(0, end + 1);
// 			start = content.find("filename=\"");
// 			if (start != std::string::npos)
// 			{
// 				start += 10;
// 				end = content.find("\"");
// 				current_connection.file_name = content.substr(start, end);
// 			}
// 		}
// 	}
// }

int	extract_IsAfterResponseClose(Connection &current_connection, std::string &header_o)
{
	std::string	header = header_o;
	std::string	str = "\r\nConnection";
	ssize_t	start = header.find(str);
	if (start == std::string::npos)
		return (0);
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == std::string::npos)
		return (0);
	std::string content = header.substr(start, end);
	std::stringstream socket_stream(content);
	std::string value;
	if (value == "close")
		current_connection.IsAfterResponseClose = 1;
	return (1);
}

int	extract_contentLength(Connection &current_connection, std::string &header_o)
{
	std::string	header = header_o;
	std::string	str = "\r\nContent-Length:";
	ssize_t	start = header.find(str);
	if (start == std::string::npos)
		return (0);
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == std::string::npos)
		return (0);
	std::string content = header.substr(start, end);
	std::stringstream socket_stream(content);
	int value;
	socket_stream >> value;
	current_connection.contentLength = value;
	return (1);
}

int	extract_contentType(Connection &current_connection, std::string &header_o)
{
	std::string	header = header_o;
	std::string	str = "\r\nContent-Type:";
	ssize_t	start = header.find(str);
	if (start == std::string::npos)
		return (0);
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == std::string::npos)
		return (0);
	std::string content = header.substr(start, end);
	std::cout << "aaaa: " << content << std::endl;
	std::stringstream socket_stream(content);
	std::string value;
	socket_stream >> value;
	current_connection.contentType = value;
	return (1);
}

int	extract_host(Connection &current_connection, std::string &header_o)
{
	std::string	header = header_o;
	std::string	str = "\r\nHost:";
	ssize_t	start = header.find(str);
	if (start == std::string::npos)
		return (0);
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == std::string::npos)
		return (0);
	std::string content = header.substr(start, end);
	std::stringstream socket_stream(content);
	std::string value;
	socket_stream >> value;
	current_connection.host = value;
	return (1);
}

int	extract_boundary(Connection &current_connection, std::string &header_o)
{
	std::string	boundary_delim;
	std::string boundary_prefix = "boundary=";
	std::string header = header_o;

	ssize_t	pos1 = header.find(boundary_prefix);
	if (pos1 == std::string::npos)
		return (0);
	pos1 += boundary_prefix.length();
	ssize_t	pos2 = header.find("\r\n", pos1);
	if (pos2 == std::string::npos)
		return (0);
	boundary_delim = "--" + header.substr(pos1, pos2 - pos1);
	current_connection.boundary = boundary_delim;
	return (1);
}

// int extract_cookies();
// int extract_authorization

int	header_extract(Connection &current_connection, std::string &header_o)
{
	extract_IsAfterResponseClose(current_connection, header_o);
	extract_contentLength(current_connection, header_o);
	extract_contentType(current_connection, header_o);
	extract_host(current_connection, header_o);
	extract_boundary(current_connection, header_o);
	return (1);
}


int	request_line(Server &server, Connection &current_connection)
{
	ssize_t		request_line_end;
	std::string &method = current_connection.method;
	std::string &URI = current_connection.URI;
	std::string &HTTP_version = current_connection.HTTP_version;
	std::string &have_read = current_connection.have_read;

	request_line_end = current_connection.have_read.find("\r\n");
	if (request_line_end == std::string::npos)
		return (set_errNbr(current_connection, 400, 0, 0, 1));

	std::string request_line = have_read.substr(0, request_line_end);
	current_connection.have_read.erase(0, request_line_end);
	std::stringstream socket_stream(request_line);
	if (!(socket_stream >> method >> URI >> HTTP_version) || HTTP_version != "HTTP/1.1" || URI == "")
		return (set_errNbr(current_connection, 400, 0, 0, 1));
	// location loc = get_location(server.locations, URI);
	// current_connection.URI = get_path_to_file(loc, URI);
	std::cout << "{" << current_connection.method << "} method" << std::endl;
	std::cout << "{" << current_connection.URI << "} URI" << std::endl;
	std::cout << "{" << current_connection.HTTP_version << "} HTTP_version" << std::endl;
	// if (!isAllowed(loc, current_connection.method))
	// 	return (set_errNbr(current_connection, 400, 0, 0, 1));
	return (1);
}


int	request_header(Server &server, Connection &current_connection)
{
	std::string &have_read = current_connection.have_read;
	
	ssize_t	header_end = have_read.find("\r\n\r\n");
	if (header_end == std::string::npos)
	{
		if (current_connection.have_read.length() > 2000)
			return (set_errNbr(current_connection, 431, 0, 0, 1)); //Request field header too large
		return (-1);
	}
	if (request_line(server, current_connection) == -1)
		return (-1);

	std::string	header;
	header = current_connection.have_read.substr(0, header_end);

	if (header_extract(current_connection, header) == -1)
		return (-1);
	if (current_connection.URI.length() > 1000)
		return (set_errNbr(current_connection, 414, 0, 0, 1));
	current_connection.have_read.erase(0, header_end + 4);
	std::cout << current_connection.errNbr << ": errNbr" << std::endl;
	std::cout << current_connection.IsAfterResponseClose << ": IsAfterResponseClose" << std::endl;
	std::cout << current_connection.socket_fd << ": socket_fd" << std::endl;
	std::cout << current_connection.isReadingHeader << ": isReadingHeader" << std::endl;
	std::cout << current_connection.isWriting << ": isWriting" << std::endl;
	std::cout << current_connection.fdWritingTo << ": fdWritingTo" << std::endl;
	
	std::cout << current_connection.host << ": host" << std::endl;
	std::cout << current_connection.contentType << ": contentType" << std::endl;
	std::cout << current_connection.form_name << ": form_name" << std::endl;
	std::cout << current_connection.file_name << ": file_name" << std::endl;
	std::cout << current_connection.have_read << ": have_read" << std::endl;
	return (1);
}
