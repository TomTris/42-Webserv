#include "../Tomweb.hpp"

int	set_errNbr(Connection &current_connection, int nbr, int isReadingHeader, int isReadingBody, int isWriting)
{
	if (nbr > 400)
	{
		current_connection.IsAfterResponseClose = 1;
	}
	current_connection.errNbr = nbr;
	current_connection.isReadingHeader = isReadingHeader;
	current_connection.isWriting = isWriting;

	return (-1);
}

int	socket_read(Connection &current_connection, std::vector<int> &to_del)
{
	int		check;
	char	buffer[5000];

	check = recv(current_connection.socket_fd, buffer, 5, MSG_PEEK);
	if (check == 0)
	{
		to_del.push_back(current_connection.socket_fd);
		current_connection.doesClientClosed = 1;
	}
	else if (check == -1)
	{
		return (set_errNbr(current_connection, 500, 0, 0, 0));
	}
	check = read(current_connection.socket_fd, buffer, sizeof(buffer) - 1);
	if (check == -1)
		return (set_errNbr(current_connection, 500, 0, 0, 0));
	buffer[check] = 0;
	current_connection.have_read += buffer;
	return (1);
}

int	connection_read(Server &server, Connection &current_connection, std::vector<int> &to_del)
{
	int	check;

	if (socket_read(current_connection, to_del) == -1)
		return (-1);
	if (current_connection.isReadingHeader == 1)
	{
		current_connection.isReadingHeader = 0;
		current_connection.isWriting = 1;
		if (request_header(server, current_connection) == -1)
			return (-1);
		if (current_connection.method == "GET" || current_connection.method == "DELETE")
		{
			if (current_connection.contentLength != 0)
				return (set_errNbr(current_connection, 400, 0, 0, 1));
			if (current_connection.method == "DELETE")
				return (deleter(current_connection));
			return (1);
		}
		if (current_connection.method == "POST")
		{
			if (current_connection.contentType != "multipart/form-data"
				|| (current_connection.form_name != "upload_form" 
					&& current_connection.form_name != "data_form"))
				return (set_errNbr(current_connection, 400, 0, 0, 1));
			if (current_connection.contentLength == 0)
				return (set_errNbr(current_connection, 411, 0, 0, 1));
			return (1);
		}
		return (set_errNbr(current_connection, 405, 0, 0, 1));
	}
	return (1);
}
