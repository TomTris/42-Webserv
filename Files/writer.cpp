#include "../Tomweb.hpp"

int	writer_handle(int code, int socket_fd, std::vector<int> &to_del)
{
	std::string getter;

	switch (code)
	{
		case 0:
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
			return (writer_handle(400, current_connection.socket_fd, to_del));
		if (!(info.st_mode & S_IRUSR))
			return (writer_handle(403, current_connection.socket_fd, to_del));
		std::cerr << "This task: listing files in a directory: haven't done yet" << std::endl;
		writer_handle(200, current_connection.socket_fd, to_del);
		if (current_connection.IsAfterResponseClose == 1)
			current_connection.reset();
		else
			to_del.push_back(current_connection.socket_fd);
		return (1);
	}
	else if (S_ISREG(info.st_mode))
	{
		if (info.st_mode & S_IRUSR)
		{
			current_connection.fdReadingFrom = open(current_connection.URI.c_str(), O_RDONLY);
			if (current_connection.fdReadingFrom == -1)
				return (writer_handle(500, current_connection.socket_fd, to_del));
			return (1);
		}
		else
			return (writer_handle(403, current_connection.socket_fd, to_del));
	}
}

int	connection_write(Server &server, Connection &current_connection, std::vector<int> &to_del)
{
	std::string statusCode;
	char buffer[5000];
	int	bytes_read;
	int fd;
	struct stat info;
	const char	*file_name;
	int			stat_val;

	if (current_connection.isReadingHeader == 1)
		return (writer_handle(400, current_connection.socket_fd, to_del));

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
			return (writer_handle(current_connection.errNbr, current_connection.socket_fd, to_del));
		if (current_connection.fdReadingFrom != -1)
		{
			bytes_read = read(current_connection.fdReadingFrom, buffer, 4999);
			if (bytes_read == -1)
			{
				close(current_connection.fdReadingFrom);
				return (writer_handle(500, current_connection.socket_fd, to_del));
			}
			buffer[bytes_read] = 0;
			if (write(fd, buffer, strlen(buffer)) == -1)
			{
				close(current_connection.fdReadingFrom);
				to_del.push_back(current_connection.socket_fd);
				return (-1);
			}
			if (bytes_read < 4999)
			{
				close(current_connection.fdReadingFrom);
				if (current_connection.IsAfterResponseClose == 1)
					to_del.push_back(current_connection.socket_fd);
				else
					current_connection.reset();
				return (1);
			}
		}
		file_name = current_connection.URI.c_str();
		stat_val = stat(file_name, &info);
		if (stat_val == -1)
			return (stat_value_non_0(current_connection, to_del));
		return (stat_value_0(info, current_connection, to_del));
	}
	return (1);
}
