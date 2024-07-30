#include "../Tomweb.hpp"

void	del_connect(Server &server, Connection &cnect, int j, std::vector<struct pollfd> &fds)
{
	std::cerr << "A CONNECTION DELETED" << std::endl;

	int fd1 = cnect.socket_fd;
	int	fd2 = cnect.reader.fdReadingFrom;
	int	fd3 = cnect.reader.writer.fdWritingTo;
	
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
	// sleep(1);
}

int	reading_done(Connection &cnect)
{
	cnect.readingHeaderDone = 1;
	cnect.reader.readingDone = 0;
	cnect.reader.time_out = clock();
	if (cnect.reader.errNbr >= 400)
	{
		cnect.IsAfterResponseClose = 1;
		return (1);
	}
	if (cnect.reader.method == "GET" || cnect.reader.method == "DELETE")
	{
		if (cnect.reader.contentLength > 0)
			cnect.reader.errNbr = 400;
	}
	if (cnect.reader.method == "POST" && cnect.reader.errNbr < 300)
	{
		std::cout << "connection level, contentleng = " << cnect.reader.contentLength << std::endl;
		if (cnect.reader.contentLength < 0)
			cnect.reader.errNbr = 400;
		else
		{
			cnect.reader.have_read = cnect.have_read;
			cnect.have_read = "";
		}
	}
	return (1);
}


int	extract_IsAfterResponseClose(std::string &header_o)
{
	std::string	header = header_o;
	std::string	str = "\r\nConnection: ";
	ssize_t	start = header.find(str);
	if (start == std::string::npos)
		return (1);
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == std::string::npos)
		return (1);
	std::string content = header.substr(start, end);
	
	if (content.find("keep-alive") == std::string::npos
		|| content.find("keep-alive") >= 10)
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
	return (value);
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
// 	std::cerr << "aaaa: " << content << std::endl;
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
	// std::cerr << "requesT_line= " << request_line << std::endl;
	std::stringstream socket_stream(request_line);
	std::string method = "";
	std::string URI = "";
	std::string HTTP_version = "";
	if (!(socket_stream >> method >> URI >> HTTP_version))
		return (cnect.reader.errNbr = 400, reading_done(cnect));
	// std::cerr << "{" << method << "} method" << std::endl;
	// std::cerr << "{" << URI << "} URI" << std::endl;
	if (method != "GET" && method != "POST" && method != "DELETE")
		return (cnect.reader.errNbr = 405, reading_done(cnect));
	if (URI.length() > 60)
		return (cnect.reader.errNbr = 400, reading_done(cnect)); // need to change
	if (HTTP_version != "HTTP/1.1")
		return (cnect.reader.errNbr = 400, reading_done(cnect)); // need to change
	cnect.reader.method = method;
	cnect.reader.URI = URI;
	cnect.have_read.erase(0, request_line_end);
	// std::cerr << URI << std::endl;
	// std:: string host = extract_host(cnect, header_o);
	// if (host != g)
	//return (cnect.reader.errNbr = 400, reading_done(cnect)); // need to change
	return (1);
}


//WARING WARNING WARNING:
//need to handle, so that 1 space enter also isn't allowed, must separate requestline and header part.
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
	header_end = cnect.have_read.find("\r\n\r\n");
	cnect.have_read.erase(0, header_end + 4);
	return (reading_done(cnect));
}

int reading_header(Server &server, Connection &connect, std::vector<struct pollfd> &fds)
{
	int		check;
	char	buffer[BUFFERSIZE + 1];

	if (check_fds(fds, connect.socket_fd) == POLL_IN)
	{
		check = read(connect.socket_fd, buffer, BUFFERSIZE);
		if (check == -1)
			return (connect.reader.errNbr = 500, reading_done(connect));
		if (check == 0)
			return (2);
		std::string a;
		a.append(buffer, check);
		std::cout << a << std::endl;
		// sleep(3);
		connect.have_read.append(buffer, check);
	}
	return (request_header(server, connect));
}

void	connection_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	// std::cerr << "connection level" << std::endl;
	time_t	now;
	for (int i = 0; i < servers.size(); i++)
	{
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			// std::cout << "servers[i].connections[j] = " << servers[i].connections[j].socket_fd << std::endl;
			// std::cout << "servers[i].connections[j].reader.cnect_close = " << servers[i].connections[j].reader.cnect_close << std::endl;
			// std::cout << "servers[i].connections[j].IsAfterResponseClose" << servers[i].connections[j].IsAfterResponseClose << std::endl;
			// std::cout << "servers[i].connections[j].reader.readingDone" << servers[i].connections[j].reader.readingDone << std::endl;
			// std::cout << "servers[i].connections[j].readingHeaderDone" << servers[i].connections[j].readingHeaderDone << std::endl;
			// std::cout << "servers[i].connections[j].reader.contentLength = " << servers[i].connections[j].reader.contentLength << std::endl;
			// usleep(500000);
			now = clock();
			// std::cout << (double) now << std::endl;
			if (servers[i].connections[j].readingHeaderDone == 0
				&& check_fds(fds, servers[i].connections[j].socket_fd) != POLLIN
				&& (now - servers[i].connections[j].time_out) / 1000 >= TIME_OUT)
			{
				std::cout << "time_out deleted" << std::endl;
				servers[i].connections[j].readingHeaderDone = 1;
				servers[i].connections[j].reader.errNbr = 408;
				servers[i].connections[j].reader.method = "";
			}
			else if (servers[i].connections[j].reader.cnect_close == 1
				|| (servers[i].connections[j].IsAfterResponseClose == 1 && servers[i].connections[j].reader.readingDone == 1))
			{
				std::cout << "request done" << std::endl;
				std::cout << "(servers[i].connections[j].reader.cnect_close " << servers[i].connections[j].reader.cnect_close << std::endl;
				std::cout << "servers[i].connections[j].IsAfterResponseClose " << servers[i].connections[j].IsAfterResponseClose << std::endl;
				std::cout << "servers[i].connections[j].reader.readingDone "  << servers[i].connections[j].reader.readingDone << std::endl;
				del_connect(servers[i], servers[i].connections[j], j, fds);
				j--;
			}
			else if (servers[i].connections[j].reader.readingDone == 1)
			{
				servers[i].connections[j].reset();
			}
			else if (servers[i].connections[j].readingHeaderDone == 0)
			{
				if (reading_header(servers[i], servers[i].connections[j], fds) == 2)
				{
					std::cout << "connection closed" << std::endl;
					del_connect(servers[i], servers[i].connections[j], j, fds);
					j--;
				}
			}
		}
	}
}
