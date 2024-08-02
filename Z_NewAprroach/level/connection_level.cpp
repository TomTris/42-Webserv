#include "../Tomweb.hpp"

void	del_connect(Server &server, Connection &cnect, int j, std::vector<struct pollfd> &fds)
{
	// std::cerr << "A CONNECTION DELETED" << std::endl;
	// std::cout << "cnect.have_read = " << cnect.have_read << std::endl;
	// std::cout << "cnect.IsAfterResponseClose = " << cnect.IsAfterResponseClose << std::endl;
	
	// std::cout << "cnect.socket_fd = " << cnect.socket_fd << std::endl;
	// std::cout << "cnect.reader.autoIndex = " << cnect.reader.autoIndex << std::endl;
	// std::cout << "cnect.reader.contentLength = " << cnect.reader.contentLength << std::endl;
	// std::cout << "cnect.reader.errFuncCall = " << cnect.reader.errFuncCall << std::endl;
	// std::cout << "cnect.reader.errNbr = " << cnect.reader.errNbr << std::endl;
	// std::cout << "cnect.reader.fdReadingFrom = " << cnect.reader.fdReadingFrom << std::endl;
	// std::cout << "cnect.reader.have_read = " << cnect.reader.have_read << std::endl;
	// std::cout << "cnect.reader.have_read_2 = " << cnect.reader.have_read_2 << std::endl;
	// std::cout << "cnect.reader.host = " << cnect.reader.host << std::endl;
	// std::cout << "cnect.reader.method = " << cnect.reader.method << std::endl;
	// std::cout << "cnect.reader.openFile = " << cnect.reader.openFile << std::endl;
	// std::cout << "cnect.reader.post = " << cnect.reader.post << std::endl;
	// std::cout << "cnect.reader.URI = " << cnect.reader.URI << std::endl;
	// std::cout << "cnect.reader.writer.fdWritingTo = " << cnect.reader.writer.fdWritingTo << std::endl;
	// std::cout << "cnect.reader.writer.writeString = " << cnect.reader.writer.writeString << std::endl;
	// std::cout << "--------------------------- " << std::endl;
	// std::cout << "cnect.reader.cnect_close = " << cnect.reader.cnect_close << std::endl;
	// std::cout << "cnect.reader.readingDone = " << cnect.reader.readingDone << std::endl;
	// std::cout << "cnect.readingHeaderDone = " << cnect.readingHeaderDone << std::endl;
	// std::cout << "cnect.reader.writer.writingDone = " << cnect.reader.writer.writingDone << std::endl;
	// std::cout << "------------------------------------------------------------------------------------------------------------ " << std::endl;
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

int	reading_done(Server &server, Connection &cnect, Reader &reader)
{
	cnect.readingHeaderDone = 1;
	cnect.reader.readingDone = 0;
	cnect.reader.time_out = clock();
	// std::cout << "reader.host = {" << reader.host << "}" << std::endl;
	// std::cout << "reader.method = {" << reader.method << "}" << std::endl;
	// std::cout << "cnect.have rad = " << cnect.have_read << std::endl;
	// std::cout << "reader.URI = {" << reader.URI << "}" << std::endl;
	// std::cout << "a[0] = " << a[0] << std::endl;
	// std::cout << "a[1] = " << a[1] << std::endl;
	// std::cout << "a[4] = " << a[4] << std::endl;
	if (reader.method != "GET" && reader.method != "POST" && reader.method != "DELETE")
		return (reader.errNbr = 405, 1);
	if (reader.URI.length() > 160)
		return (reader.errNbr = 414, 1);
	std::vector<std::string> a = get_data(reader.host, reader.method, reader.URI, server);
	//URI
	if (*reader.URI.begin() != '/')
		reader.URI = "/" + reader.URI;
	reader.URI = a[2];
	// std::cout << "a[2] = " << reader.URI << std::endl;
	// std::cout << "a[3] = " << a[3] << std::endl;
	//a[0] is host:post ok?
	// std::cout << "cnect.reader.errNbr = " << cnect.reader.errNbr << std::endl;
	if (a[0] == "0")
		return (reader.errNbr = 400, 1);
	else if (reader.contentLength > server.body_size_max)
		return (reader.errNbr = 413, 1);
	else if (a[1] == "0") //method not allowed
		return (reader.errNbr = 405, 1);
	else
	{
		reader.autoIndex = 1;
		if (a[3] == "0") //auto index
			reader.autoIndex = 0;
	}
	if (a[4] != "")
		return (reader.URI = a[4], cnect.reader.errNbr = 301, 1);
	if (cnect.reader.method == "GET" || cnect.reader.method == "DELETE")
	{
		if (cnect.reader.contentLength > 0)
			cnect.reader.errNbr = 400;
		return (1);
	}
	if (cnect.reader.method == "POST" && cnect.reader.errNbr < 300)
	{
		if (cnect.reader.contentLength < 0)
			cnect.reader.errNbr = 400;
		else
		{
			cnect.reader.have_read = cnect.have_read;
			cnect.have_read = "";
		}
		return (1);
	}
	// std::cout << "connection level 405" << std::endl;
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
		return ("");
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	if (end == std::string::npos)
		return ("");
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
	cnect.reader.host = extract_host(cnect, header_o);
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
		return (cnect.reader.errNbr = 400, cnect.readingHeaderDone = 1, 1);
	// std::cerr << "{" << method << "} method" << std::endl;
	// std::cerr << "{" << URI << "} URI" << std::endl;
	if (method != "GET" && method != "POST" && method != "DELETE")
		return (cnect.reader.errNbr = 405, cnect.readingHeaderDone = 1, 1);
	if (HTTP_version != "HTTP/1.1")
	{
		// std::cout << "HTTP-version = " << HTTP_version << std::endl;
		return (cnect.reader.errNbr = 400, cnect.readingHeaderDone = 1, 1); // need to change
	}
	cnect.reader.method = method;
	cnect.reader.URI = URI;
	// cnect.have_read.erase(0, request_line_end);
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
	// while (1)
	// {
	// 	if (*cnect.have_read.begin() == '\r' && *(cnect.have_read.begin() + 1) == '\n')
	// 		cnect.have_read.erase(0, 2);
	// 	else
	// 		break ;
	// }
	if (cnect.have_read.length() == 0)
		return (1);
	// sleep(1);
	ssize_t	header_end = cnect.have_read.find("\r\n\r\n");
	if (header_end == std::string::npos)
	{
		if (cnect.have_read.length() > 2000)
			return (cnect.readingHeaderDone = 1, cnect.IsAfterResponseClose = 1, cnect.reader.errNbr = 431);
		return (0);
	}
	// std::cout << "000000000000000" << std::endl;
	// sleep(1);
	// std::cout << "{" << cnect.have_read.substr(0, header_end) << "}" << std::endl;
	// if (request_line(server, cnect) == -1)
	// 	return (1);
	request_line(server, cnect);
	header_extract(cnect, cnect.have_read);
	header_end = cnect.have_read.find("\r\n\r\n");
	cnect.have_read.erase(0, header_end + 4);
	return (reading_done(server, cnect, cnect.reader));
}

int reading_header(Server &server, Connection &connect, std::vector<struct pollfd> &fds)
{
	int		check;
	char	buffer[BUFFERSIZE + 1];

	if (check_fds(fds, connect.socket_fd) == POLLIN)
	{
		check = read(connect.socket_fd, buffer, BUFFERSIZE);
		if (check == -1)
		{
			// std::cout << "Check = -1, fd = " << connect.socket_fd << std::endl;
			// return (1);
			return (connect.reader.errNbr = 500, connect.readingHeaderDone = 1, 1);
		}
		if (check == 0)
			return (2);
		int check2 = check;
		// std::cout << "fd = " << connect.socket_fd << std::endl;
		std::string a = "";
		// << "\nhere have read before = {" << connect.have_read << "}\n"<< std::endl;
		a.append(buffer, check);
		// std::cout << "\n\n--------------------------------------------------------";
		// std::cout << "++++++++++++++++++++++++" << std::endl;
		std::cout << a << std::endl;
		// std::cout << "++++++++++++++++++++++++" << std::endl;
		// sleep(1);
		connect.have_read.append(buffer, check);
		// std::cout << "here have read  after1 = {" << connect.have_read << "}\n--------------------------------------------------------\n\n"<< std::endl;
		// sleep(2);
	}
	// std::cout << "Here it should be 0: " << connect.readingHeaderDone << std::endl;
	return (request_header(server, connect));
}

void	connection_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	time_t	now;
	for (int i = 0; i < servers.size(); i++)
	{
		// std::cout << "++++++++++" << " < servers[i].connections.size()" << servers[i].connections.size()<< "++++++++" << std::endl;
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			// std::cout << "++1111++++++++++++++++" << std::endl;
			// std::cout << "--a-" << std::endl;
			// std::cout << "reader.method " << servers[i].connections[j].reader.method << std::endl;
			// std::cout << "reader.URI " << servers[i].connections[j].reader.URI << std::endl;
			// std::cout << "reader.errNbr " << servers[i].connections[j].reader.errNbr << std::endl;
			// std::cout << "cnect.readingHeaderDone" << servers[i].connections[j].readingHeaderDone << std::endl;
			// std::cout << "---" << std::endl;
			now = clock();
			if (servers[i].connections[j].readingHeaderDone == 0
				&& check_fds(fds, servers[i].connections[j].socket_fd) != POLLIN
				&& (now - servers[i].connections[j].time_out) / 1000000 >= TIME_OUT)
			{
				servers[i].connections[j].readingHeaderDone = 1;
				servers[i].connections[j].reader.errNbr = 408;
				servers[i].connections[j].reader.method = "GET";
			}
			else if (servers[i].connections[j].reader.cnect_close == 1
				|| (servers[i].connections[j].IsAfterResponseClose == 1 && servers[i].connections[j].reader.writer.writingDone == 1))
			{
				del_connect(servers[i], servers[i].connections[j], j, fds);
				j--;
			}
			else if (servers[i].connections[j].reader.writer.writingDone == 1)
			{
				servers[i].connections[j].reset();
			}
			else if (servers[i].connections[j].readingHeaderDone == 0)
			{
				// std::cout << " aaaaai = " << i << std::endl;
				// std::cout << " aaaaaj = " << j << std::endl;
				if (reading_header(servers[i], servers[i].connections[j], fds) == 2)
				{
					del_connect(servers[i], servers[i].connections[j], j, fds);
					j--;
				}
				// std::cout << " bbbbbi = " << i << std::endl;
				// std::cout << " bbbbbj = " << i << std::endl;
				// sleep(1);
			}
		}
	}
}
