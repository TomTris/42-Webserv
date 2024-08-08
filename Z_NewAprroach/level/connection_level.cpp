#include "../Tomweb.hpp"

void	del_connect(Server &server, Connection &cnect, int j)
{
	std::cerr << "DELETED " <<  cnect.socket_fd << std::endl;

	cnect.reset();
	close(cnect.socket_fd);
	remove_from_poll(cnect.socket_fd);
	if (server.connections.size() == 1)
		server.connections.clear();
	else
		server.connections.erase(server.connections.begin() + j);
}

int	get_data_from_parsing(Server &server, Connection &cnect, Reader &reader)
{
	std::vector<std::string> a = get_data(reader.host, reader.method, reader.URI, server);

	if (a[0] == "0") //host:post
		return (reader.errNbr = 400, 1);
	else if (reader.contentLength > server.body_size_max)
		return (reader.errNbr = 413, 1);
	else if (a[1] == "0") //method not allowed
		return (reader.errNbr = 405, 1);
	
	reader.URI = a[2];
	if (*reader.URI.begin() != '/')
		reader.URI = '/' + reader.URI;
	reader.CGI_path = reader.URI;
	reader.autoIndex = 1;

	if (a[3] == "0") //auto index
		reader.autoIndex = 0;

	if (a[4] != "") //redirect
		return (reader.URI = a[4], cnect.reader.errNbr = 301, 1);

	cnect.reader.cgi_ex = a[5];
	return (0);
}

int	reading_done_precheck(Connection &cnect, Reader &reader)
{
	cnect.readingHeaderDone = 1;
	cnect.reader.readingDone = 0;
	cnect.reader.time_out = time(0);

	if (reader.method != "GET" && reader.method != "POST" && reader.method != "DELETE")
		return (reader.errNbr = 405, 1);
	if (reader.URI.length() > 500)
		return (reader.errNbr = 414, 1);
	while (*reader.URI.begin() == '/')
		reader.URI.erase(reader.URI.begin());
	reader.URI.append("/");
	
	reader.query_string = "";
	if (reader.URI.find("?") != std::string::npos)
	{
		reader.query_string = reader.URI.substr(reader.URI.find("?") + 1);
		reader.URI = reader.URI.substr(0, reader.URI.find("?"));
	}
	return (0);
}

int	reading_done_last_check(Connection &cnect)
{
	if (cnect.reader.method == "GET" || cnect.reader.method == "DELETE")
		if (cnect.reader.contentLength > 0)
			return (cnect.reader.errNbr = 400, 1);
	if (cnect.reader.method == "POST" && cnect.reader.errNbr < 300)
	{
		if (cnect.reader.contentLength < 0)
			return (cnect.reader.errNbr = 400, 1);
		cnect.reader.have_read_2 = cnect.have_read;
		cnect.have_read = "";
	}
	if (cnect.reader.URI.find("/cgi-bin/") == 0)
		cnect.reader.readCGI = 1;
	if (cnect.reader.readCGI == 1
		&& (cnect.reader.method != "GET" && cnect.reader.method != "POST"))
		return (cnect.reader.errNbr = 400, 1);
	return (1);
}
int	reading_done(Server &server, Connection &cnect, Reader &reader)
{
	if (reading_done_precheck(cnect, reader) == 1
		|| get_data_from_parsing(server, cnect, reader) == 1
		|| reading_done_last_check(cnect))
		return (1);
	return (1);
}

int	request_line(Connection &cnect)
{
	ssize_t		request_line_end;

	while (cnect.have_read.length() > 0 && cnect.have_read.find("\r\n") == 0)
	{
		if (cnect.have_read.length() == 2)
			cnect.have_read = "";	
		else
			cnect.have_read.erase(0, 2);
	}
	if (cnect.have_read.length() == 0)
		return 0;
	cnect.reader.request_line_done = 1;
	request_line_end = cnect.have_read.find("\r\n");

	std::string request_line = cnect.have_read.substr(0, request_line_end);
	std::stringstream socket_stream(request_line);
	std::string method = "";
	std::string URI = "";
	std::string HTTP_version = "";

	if (!(socket_stream >> method >> URI >> HTTP_version))
		return (cnect.reader.errNbr = 400, cnect.readingHeaderDone = 1, 1);
	if (method != "GET" && method != "POST" && method != "DELETE")
		return (cnect.reader.errNbr = 405, cnect.readingHeaderDone = 1, 1);
	if (HTTP_version != "HTTP/1.1")
		return (cnect.reader.errNbr = 400, cnect.readingHeaderDone = 1, 1);
	cnect.reader.method = method;
	cnect.reader.URI = URI;
	cnect.have_read.erase(0, cnect.have_read.find("\r\n"));
	return (1);
}

//WARING WARNING WARNING:
//need to handle, so that 1 space enter also isn't allowed, must separate requestline and header part.
int	request_header(Server &server, Connection &cnect)
{
	if (cnect.reader.request_line_done == 0 && cnect.have_read.find("\r\n") != std::string::npos)
		if (request_line(cnect) == 0)
			return (0);
	ssize_t	header_end = cnect.have_read.find("\r\n\r\n");
	if (header_end == static_cast<ssize_t>(std::string::npos))
	{
		if (cnect.have_read.length() > 2000)
			return (cnect.readingHeaderDone = 1, cnect.IsAfterResponseClose = 1, cnect.reader.errNbr = 431);
		return (0);
	}
	header_extract(cnect, cnect.have_read);
	header_end = cnect.have_read.find("\r\n\r\n");
	cnect.have_read.erase(0, header_end + 4);
	return (reading_done(server, cnect, cnect.reader));
}

int reading_header(Server &server, Connection &connect)
{
	int		check;
	char	buffer[BUFFERSIZE + 1];

	if (check_fds(connect.socket_fd) & POLLIN)
	{
		check = read(connect.socket_fd, buffer, BUFFERSIZE);
		if (check == -1)
			return (printf("reader in connection = -1\n"),1);
		if (check == 0)
			return (printf("returns 2\n") ,2);
		std::cout << buffer << std::endl;
		connect.have_read.append(buffer, check);
	}
	return (request_header(server, connect));
}

void	connection_level(std::vector<Server> &servers)
{
	time_t	now;
	int	revents;
	unsigned int	j;
	Connection 		*cnect;

	for (unsigned int i = 0; i < servers.size(); i++)
	{
		j = 0;
		while (j < servers[i].connections.size())
		{
			cnect = &servers[i].connections[j];
			revents = check_fds(servers[i].connections[j].socket_fd);
			now = time(0);
			if (cnect->reader.cnect_close == 1
				|| (cnect->IsAfterResponseClose == 1 && cnect->reader.writer.writingDone == 1)
				|| revents & POLLHUP)
			{
				// std::cout << "Maybe revents = POLLHUP" << std::endl;
				del_connect(servers[i], *cnect, j);
			}
			else if (cnect->reader.writer.writingDone == 1 )
			{
				cnect->reset();
				j++;
			}
			else if (cnect->readingHeaderDone == 0
				&& revents != POLLIN
				&& difftime(now, cnect->time_out) >= 15)
			{
				cnect->readingHeaderDone = 1;
				cnect->reader.errNbr = 408;
				j++;
				// del_connect(servers[i], *cnect, j);
			}
			else if (cnect->readingHeaderDone == 0)
			{
				if (reading_header(servers[i], *cnect) == 2)
					del_connect(servers[i], *cnect, j);
				else
					j++;
			}
			else
				j++;
		}
	}
}

// #define RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\n"
// void	connection_level(std::vector<Server> &servers)
// {
// 	unsigned int	j;

// 	for (unsigned int i = 0; i < servers.size(); i++)
// 	{
// 		j = 0;
// 		while (j < servers[i].connections.size())
// 		{
// 			if (check_fds(servers[i].connections[j].socket_fd) == POLLOUT)
// 			{
// 				write(servers[i].connections[j].socket_fd, RESPONSE, strlen(RESPONSE));
// 				close(servers[i].connections[j].socket_fd);
// 				remove_from_poll(servers[i].connections[j].socket_fd);
// 			}
// 			else
// 				j++;
// 		}
// 	}
// }
