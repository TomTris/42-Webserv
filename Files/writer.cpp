#include "../Tomweb.hpp"
void writer_200(int &new_socket, char *str);
int	writer_handle(int code, int socket_fd, std::vector<int> &to_del)
{
	std::string getter;
	std::cout << "writer handle called, code = " << code << "socket = " << socket_fd << std::endl;
	switch (code)
	{
		case 0:
			getter = handle_0();
			break ;
		case 200:
			getter = handle_0();
			break ;
		case 204:
			getter = handle_204();
			break ;
		case 400:
			getter = handle_400();
			break ;
		case 401:
			getter = handle_401();
			break ;
		case 403:
			getter = handle_403();
			break ;
		case 404:
			getter = handle_404();
			break ;
		case 408:
			getter = handle_408();
			break ;
		case 409:
			getter = handle_409();
			break ;
		case 411:
			getter = handle_411();
			break ;
		default:
			getter = handle_00();
	}
	write(socket_fd, getter.c_str(), getter.length());
	if (code >= 400)
	{
		to_del.push_back(socket_fd);
		return (-1);
	}
	return (1);
}

int	stat_value_non_0(Connection &current_connection, std::vector<int> &to_del)
{
	switch (errno)
	{
		case ENOENT:
			return (writer_handle(400, current_connection.socket_fd, to_del));
		case EACCES:
			return (writer_handle(401, current_connection.socket_fd, to_del));
		case ELOOP:
			return (writer_handle(508, current_connection.socket_fd, to_del));
		default:
			return (writer_handle(500, current_connection.socket_fd, to_del));
	}
}

int	stat_value_0(struct stat &info, Connection &current_connection, std::vector<int> &to_del)
{
	if (S_ISDIR(info.st_mode))
	{
		if (!current_connection.autoIndex)
		{
			return (writer_handle(400, current_connection.socket_fd, to_del));
		}
		if (!(info.st_mode & S_IRUSR))
			return (writer_handle(403, current_connection.socket_fd, to_del));
		char a[200] = "void writer_200(int &new_socket, char *str)";
		writer_200(current_connection.socket_fd, a);
		if (current_connection.IsAfterResponseClose == 1)
			current_connection.reset();
		else
			to_del.push_back(current_connection.socket_fd);
		return (1);
	}
	else if (S_ISREG(info.st_mode))
	{
		if (!(info.st_mode & S_IRUSR))
			return (writer_handle(403, current_connection.socket_fd, to_del));
		current_connection.fdReadingFrom = open(current_connection.URI.c_str(), O_RDONLY);
		if (current_connection.fdReadingFrom == -1)
			return (writer_handle(500, current_connection.socket_fd, to_del));
		return (1);
	}
	else
	{
		return (writer_handle(403, current_connection.socket_fd, to_del)); // idk what it is, so, permisison denied =))
	}
}
void writer_200(int &new_socket, char *str)
{
	std::cout << "writer 200 called " << std::endl;

    // Calculate the length of the response body
    std::string body = "<html><body>" + std::string(str) + "</body></html>";
    size_t content_length = body.length();

    // Headers
    std::ostringstream header;
    header << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << content_length << "\r\n"
           << "\r\n";
    
    // Write headers
    write(new_socket, header.str().c_str(), header.str().size());
    
    // Write body
    write(new_socket, body.c_str(), body.length());
}

int	connection_write(Server &server, Connection &current_connection, std::vector<int> &to_del)
{
	std::string statusCode;
	char buffer[5000000];
	int	bytes_read;
	struct stat info;
	const char	*file_name;
	int			stat_val;
	std::cout << "writer, curernt_connetion: " << current_connection.socket_fd << std::endl;
	if (current_connection.errNbr >= 400)
	{
		return (writer_handle(current_connection.errNbr, current_connection.socket_fd, to_del));
	}
	if (current_connection.isReadingHeader == 1)
	{
		int fd = open("favicon.ico", O_RDONLY);
		std::cout << "open favicon = " << fd << std::endl;
		bytes_read = read (fd, buffer, 4999999);
		if (bytes_read == -1)
			return (writer_handle(500, current_connection.socket_fd, to_del));
		buffer[bytes_read] = 0;
		close(fd);
		writer_200(current_connection.socket_fd, buffer);
		if (current_connection.IsAfterResponseClose == 1)
			to_del.push_back(current_connection.socket_fd);
		else
			current_connection.reset();
		std::cout << 1 << std::endl;
		return (1);
	}

	if (current_connection.method == "POST")
		return (body_handle_post(current_connection));
	if (current_connection.method == "DELETE")
	{
		writer_handle(current_connection.errNbr, current_connection.socket_fd, to_del);
		if (current_connection.errNbr < 400)
		{
			if (current_connection.IsAfterResponseClose == 1)
				to_del.push_back(current_connection.socket_fd);
			else
				current_connection.reset();
		}
		return (1);
	}
	if (current_connection.method == "GET")
	{
		if (current_connection.errNbr >= 400)
		{
			return (writer_handle(current_connection.errNbr, current_connection.socket_fd, to_del));
		}
		if (current_connection.fdReadingFrom != -1)
		{
			bytes_read = read(current_connection.fdReadingFrom, buffer, 4999);
			if (bytes_read == -1)
			{
				close(current_connection.fdReadingFrom);
				return (writer_handle(500, current_connection.socket_fd, to_del));
			}
			buffer[bytes_read] = 0;
			writer_200(current_connection.socket_fd, buffer);
			close(current_connection.fdReadingFrom);
			if (current_connection.IsAfterResponseClose == 1)
				to_del.push_back(current_connection.socket_fd);
			else
				current_connection.reset();
			return (1);
		}
		std::cout << "1{" << current_connection.URI << "}" << std::endl;
		ssize_t rm = current_connection.URI.find_first_not_of('/');
		std::cout << "2{" << current_connection.URI << "}" << std::endl;
		current_connection.URI.erase(0, rm);
		stat_val = stat(current_connection.URI.c_str(), &info);
		if (stat_val == -1)
		{
			std::cout << "stat_val = -1" << std::endl;
			std::cout << "{" << current_connection.URI << "}" << std::endl;
			return (stat_value_non_0(current_connection, to_del));
		}
		return (stat_value_0(info, current_connection, to_del));
	}
	return (1);
}
