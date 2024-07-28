#include "../Tomweb.hpp"

void	del_connect(Server &server, int j, std::vector<struct pollfd> &fds)
{
	std::cout << "A CONNECTION DELETED" << std::endl;
	int fd1 = server.connections[j].socket_fd;
	int	fd2 = server.connections[j].reader.fdReadingFrom;
	int	fd3 = server.connections[j].reader.writer.fdWritingTo;
	
	if (fd2 == fd1)
		fd2 = -1;
	if (fd3 == fd1)
		fd3 = -1;
	remove_from_poll(fd1, fds);
	close(fd1);
	
	if (fd2 != -1)
	{
		remove_from_poll(fd2, fds);
		close(fd2);
	}
	if (fd3 != -1)
	{
		remove_from_poll(fd3, fds);
		close(fd3);
	}

	server.connections.erase(server.connections.begin() + j);
}

int	reading_done(Connection &cnect)
{
	cnect.readingHeaderDone = 1;
	cnect.reader.done = 0;
	if (cnect.reader.errNbr >= 400)
	{
		cnect.IsAfterResponseClose = 1;
	}
	if (cnect.reader.method == "POST")
	{
		cnect.reader.have_read = cnect.have_read;
	}
	std::cout << "reading header done!!" << std::endl;
	return (1);
}


int	extract_IsAfterResponseClose(std::string &header_o)
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
	if (value == "keep-alive")
		return (1);
	return (0);
}

int	extract_contentLength(std::string &header_o)
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
	return (1);
}

// int	extract_contentType(Connection &current_connection, std::string &header_o)
// {
// 	std::string	header = header_o;
// 	std::string	str = "\r\nContent-Type:";
// 	ssize_t	start = header.find(str);
// 	if (start == std::string::npos)
// 		return (0);
// 	start += str.length();
// 	ssize_t end = header.find("\r\n", start);
// 	if (end == std::string::npos)
// 		return (0);
// 	std::string content = header.substr(start, end);
// 	std::cout << "aaaa: " << content << std::endl;
// 	std::stringstream socket_stream(content);
// 	std::string value;
// 	socket_stream >> value;
// 	current_connection.reader.contentType = value;
// 	return (1);
// }

std::string	extract_host(Connection &current_connection, std::string &header_o)
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
	return (value);
}

// int	extract_boundary(Connection &current_connection, std::string &header_o)
// {
// 	std::string	boundary_delim;
// 	std::string boundary_prefix = "boundary=";
// 	std::string header = header_o;

// 	ssize_t	pos1 = header.find(boundary_prefix);
// 	if (pos1 == std::string::npos)
// 		return (0);
// 	pos1 += boundary_prefix.length();
// 	ssize_t	pos2 = header.find("\r\n", pos1);
// 	if (pos2 == std::string::npos)
// 		return (0);
// 	boundary_delim = "--" + header.substr(pos1, pos2 - pos1);
// 	current_connection.reader.boundary = boundary_delim;
// 	return (1);
// }

// // int extract_cookies();
// // int extract_authorization

int	header_extract(Connection &cnect, std::string &header_o)
{
	std::string host;

	cnect.IsAfterResponseClose = extract_IsAfterResponseClose(header_o);
	cnect.reader.contentLength = extract_contentLength(header_o);
	// extract_contentType(header_o);
	//  extract_host(cnect, header_o);
	// extract_boundary(cnect, header_o)
	return (1);
}

int	request_line(Server &server, Connection &cnect)
{
	ssize_t		request_line_end;

	request_line_end = cnect.have_read.find("\r\n");

	std::string request_line = cnect.have_read.substr(0, request_line_end);
	std::stringstream socket_stream(request_line);
	std::string method = "";
	std::string URI = "";
	std::string HTTP_version = "";
	if (!(socket_stream >> method >> URI >> HTTP_version))
		return (cnect.reader.errNbr = 400, reading_done(cnect));
	std::cout << "{" << method << "} method" << std::endl;
	std::cout << "{" << URI << "} URI" << std::endl;
	std::cout << "{" << HTTP_version << "} HTTP_version" << std::endl;
	if (method != "GET" && method != "POST" && method != "DELETE")
		return (cnect.reader.errNbr = 405, reading_done(cnect));
	if (URI.length() > 60)
		return (cnect.reader.errNbr = 400, reading_done(cnect)); // need to change
	if (HTTP_version != "HTTP/1.1")
		return (cnect.reader.errNbr = 400, reading_done(cnect)); // need to change
	cnect.reader.method = method;
	cnect.reader.URI = URI;
	cnect.have_read.erase(0, request_line_end);
	std::cout << URI << std::endl;
	// std:: string host = extract_host(cnect, header_o);
	// if (host != g)
	//return (cnect.reader.errNbr = 400, reading_done(cnect)); // need to change
	return (1);
}

int	request_header(Server &server, Connection &cnect)
{
	while (1)
	{
		if (*cnect.have_read.begin() == '\r' && *(cnect.have_read.begin() + 1) == '\n')
			cnect.have_read.erase(0, 2);
		else
			break ;
	}
	ssize_t	header_end = cnect.have_read.find("\r\n\r\n");
	
	if (header_end == std::string::npos)
	{
		if (cnect.have_read.length() > 1000)
			return (cnect.reader.errNbr = 431); //Request field header too large
		return (0);
	}
	if (request_line(server, cnect) == -1)
		return (1);
	header_extract(cnect, cnect.have_read);
	return (reading_done(cnect));
}

int reading_header(Server &server, Connection &connect, std::vector<struct pollfd> &fds)
{
	int		check;
	char	buffer[BUFFERSIZE + 1];

	check = read(connect.socket_fd, buffer, BUFFERSIZE);
	if (check == -1)
		return (connect.reader.errNbr = 500, reading_done(connect));
	if (check == 0)
		return (2);
	buffer[check] = 0;
	connect.have_read += buffer;
	std::cout << "length = " << strlen(buffer) << std::endl;
	std::cout << "{"<< buffer << "}"<<std::endl;
	return (request_header(server, connect));
}

void	connection_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	std::cout << "connection level" << std::endl;
	for (int i = 0; i < servers.size(); i++)
	{
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			if ( servers[i].connections[j].reader.cnect_close == 1
				|| (servers[i].connections[j].reader.doesClientClose == 1
				&& (servers[i].connections[j].reader.method == "" || servers[i].connections[j].reader.method == "GET")))
			{
				del_connect(servers[i], j, fds);
				j--;
			}
			else if (servers[i].connections[j].readingHeaderDone == 0 && check_fds(fds, servers[i].connections[j].socket_fd) & POLL_IN)
			{
				if (reading_header(servers[i], servers[i].connections[j], fds) == 2)
				{
					del_connect(servers[i], j, fds);
					j--;
				}
			}
		}
	}
}

// void	connection_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
// {
// 	std::cout << "connection level" << std::endl;
// 	char	buffer[BUFFERSIZE + 1];
// 	int	 bytes_read;

// 	for (int i = 0; i < servers.size(); i++)
// 	{
// 		for (int j = 0; j < servers[i].connections.size(); j++)
// 		{
// 			std::cout << "cnect fd = " << servers[i].connections[j].socket_fd << std::endl;
// 			if (check_fds(fds, servers[i].connections[j].socket_fd) == 1)
// 			{
// 				bytes_read = read(servers[i].connections[j].socket_fd, buffer, BUFFERSIZE);
// 				check_fds(fds, servers[i].connections[j].socket_fd);
// 			}
// 		}
// 	}
// }
