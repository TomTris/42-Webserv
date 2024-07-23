#include "Tomweb.hpp"

int	connection_delete(int &fd_to_del, std::vector<Server> &servers)
{

	for (int i = 0; i < servers.size(); i++)
	{
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			if (servers[i].connections[j].socket_fd == fd_to_del)
			{
				servers[i].connections.erase(servers[i].connections.begin() + j);
				return (1);
			}
		}
	}
	std::cout << "connection delete somehow wrong" << std::endl;
	return (0);
}

int	connections_delete(std::vector<int> &to_del_fds, std::vector<Server> &servers)
{
	while (to_del_fds.size() > 0)
	{
		connection_delete(fd_to_del, servers);
	}
}

void	connection_accept(Server &server)
{
	int		new_socket;
	size_t	addrlen = sizeof(server.address);

	if ((new_socket = accept(server.serverFD, (struct sockaddr *)&server.address, (socklen_t*)&addrlen)) < 0)
		throw std::runtime_error("accept failed");
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl failed");
	server.to_add_fds.pushback(new_socket);

}

void connections_add(std::vector<Server> &servers)
{
	for (int i = 0; i < servers.size(); i++)
	{
		while (servers.to_add_fds.size() > 0)
		{
			servers.connections.push_back(Connection(servers.to_add_fds[0]));
			to_add_fds.erase(to_add_fds.begin());
		}
	}
}

void	connection_exception(Connection &current_connection, std::vector<int> &to_del)
{
	current_connection.isError = 1;
	std::cout << "Connection with fd = {" << current_connection.socket_fd << "} has exception" << std::endl;
	to_del.push_back(current_connection.socket_fd);
}

//added 431
//413 Payload Too Large
int	set_any_error(Connection &current_connection, int nbr)
{
	if (nbr == 400 ||nbr == 401 ||nbr == 403 ||nbr == 404 ||nbr == 408
		||nbr == 500 ||nbr == 502 ||nbr == 503 ||nbr == 504 || nbr == 431
		|| nbr == 413)
	{
		isRepsoneExit = 1;
	}
	current_connection.any_error = nbr;
	isReadingHeader = 1;
	isReadingBody = 0;
	isWriting = 0;

	return (-1);
}

int	request_line(Server &server, Connection &current_connection)
{
	ssize_t	request_line_end;

	request_line_end = current_connection.have_read.find("\r\n");
	if (request_line_end == std::string::npos)
		return (set_any_error(set_any_error, 400));

	std::string request_line = have_read.substr(0, have_read.find("\r\n"));
	have_read.erase(0, have_read.find("\r\n") + 2);
	
	std::stringstream socket_stream(request_line);
	if (!(socket_stream >> method >> URL >> HTTP_version) || HTTP_version != "HTTP/1.1" !! URL == "")
		return (set_any_error(current_connection, 400));
	location loc = get_location(server.locations, URL);
	current_connection.URL = get_path_to_file(loc, URL);
	if (!isAllowed(loc, current_connection.method))
		return (set_any_error(current_connection, 400));
	return (1);
}

int	header_extract(Connection &current_connection, std::string &header)
{
	ssize_t	isResponseCloseStart = header.find("\r\nConnection:");
	if (isResponseClose != std::string::npos)
	{
		ssize_t isResponseCloseEnd =
		std::string isResClose = header.substr(isResponseClose + 13, len - 13 - isResponseClose);
		std
	}

}

int	request_header(Server &server, Connection &current_connection)
{
	ssize_t	header_end = current_connection.find("\r\n\r\n");
	std::string	header;

	if (header_end == std::string::npos)
		return (set_any_error(current_connection, 431)); //Request field header too large
	header = current_connection.substr(0, header_end);
	if (header_extract(current_connection, header) == -1)
		return (-1);
	current_connection.erase(0, current_connection + 4);
	
}
// if (URL.length() > 30)
// 	{
// 		return (set_any_error(current_connection, 414));
// 	}


int	connection_read(Server &server, Connection &current_connection, std::vector<int> &to_del)
{
	int	check;
	char	buffer[5000] = {0};

	check = recv(socket_fd, buffer, 5, MSG_PEEK);
	if (check == 0)
	{
		if (current_connection.method == "GET")
			to_del.push_back(current_connection.socket_fd);
		return (set_any_error(current_connection, 0));
	}
	else if (check == -1)
		return (set_any_error(current_connection, 500));

	check = read(current_connection.socket_fd, buffer, sizeof(buffer) - 1);
	if (check == -1)
		return (set_any_error(current_connection, 500));
	buffer[check] = 0;
	current_connection.have_read += buffer;



	if (current_connection.isReadingHeader == 1)
	{
		isReadingHeader = 0;
		check = request_line(server, current_connection, to_del, check);
		if (check == -1)
			return check;
		return (request_header(server, current_connection, to_del, check));			
	}

	if (current_connection.isReading == 1)
	{
		if (current_connection.method == "GET")
			check = socket_header_read_get(current_connection);
		else if (current_connection.method == "POST")
			check = socket_header_read_post(current_connection);
		else if (current_connection.method == "DELETE")
			check = socket_header_read_delete(current_connection);
	}
}

// ISResponseClose == 1 && any_error != 0 => return the file err, then close.
// IsResponseClose == 1 && any_error == 0 => do the rest, then close;. it's POST/DELETE method.
// IsResponseClose == 0 && any_error == 1 => return the file, then keep doing.
// IsResponseClose == 0 && any_error == 0 => normal.



//All: Read request_line first, decide keep doing or not, which method, which url.
//Based on the error decided if close connection or not :
//400 401 403  404 408  500 502 503 504. Close after the isWriting is done, then delete himself.
//If the connection is closed
// by the client -> detlete itself.

//is readingbody is only for POST.

//GET: only HEADER. IF content-length should not exist, if exists -> must be 0.
//check closeConnection or not? So when the request finish -> add itself to_Del
//There shouldn't be body. handle only until \r\n\r\n
//There rest: next request.

//POST: HEADER - find content-length, content-type - decide the way to handle
//check closeConnection or not? So when the request finish -> add itself to_Del
//must have Content-length, content-type
//if haven't get all of length, wait for 20 circle. if not -> time out
//if write OP ready => bad request


1. recv = 0 -> have read socket to the end + connection is closed;
=> del himself.

2. if we read already till the end of the request but connection is open
=> decide close or not based on error.
if close: done.

if not close:
2.1 there is something left to read?
yes: consider as new request.
no:
2.1.2 if ready for write?
yes: 400 - Bad Request
no: keep counting circle until 50 -> timeout (about 5s);

