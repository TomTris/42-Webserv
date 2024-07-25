#include "Tomweb.hpp"

//added 431
//413 Payload Too Large
//415 Unsupported Media Type
int	isExit(int nbr)
{
	if (nbr == 400 ||nbr == 401 ||nbr == 403 ||nbr == 404 ||nbr == 408
		||nbr == 500 ||nbr == 502 ||nbr == 503 ||nbr == 504 || nbr == 431
		|| nbr == 413 || nbr == 415)
		return (1);
	return (0);
}

int	set_any_error(Connection &current_connection, int nbr, int isReadingHeader, int isReadingBody, int isWriting)
{
	if (nbr == 400 ||nbr == 401 ||nbr == 403 ||nbr == 404 ||nbr == 408
		||nbr == 500 ||nbr == 502 ||nbr == 503 ||nbr == 504 || nbr == 431
		|| nbr == 413 || nbr == 415)
	{
		current_connection.IsAfterResponseClose = 1;
	}
	else
		current_connection.needReadToTheEnd = 1;
	current_connection.any_error = nbr;
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
		if (current_connection.method == "GET")
		{
			to_del.push_back(current_connection.socket_fd);
			current_connection.doesClientClosed = 1;
		}
		
		return (set_any_error(current_connection, 0, 0, 0, current_connection.isWriting));
	}
	else if (check == -1)
		return (set_any_error(current_connection, 500, 0, 0, 0));
	
	check = read(current_connection.socket_fd, buffer, sizeof(buffer) - 1);
	if (check == -1)
		return (set_any_error(current_connection, 500, 0, 0, 0));
	buffer[check] = 0;
	current_connection.have_read += buffer;
	
	if (current_connection.needReadToTheEnd == 1)
	{
		if (check < 4999)
			current_connection.needReadToTheEnd = 0;
		current_connection.have_read = "";
		return (-1);
	}
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
		if (request_header(server, current_connection, to_del, check) == -1)
			return (-1);
		current_connection.isWriting = 1;
		current_connection.isReadingBodyHeader = 1;
		return (1);
	}
}

// IsAfterResponseClose == 1 && any_error != 0 => return the file err, then close.
// IsAfterResponseClose == 1 && any_error == 0 => do the rest, then close;. it's POST/DELETE method.
// IsAfterResponseClose == 0 && any_error == 1 => return the file, then keep doing.
// IsAfterResponseClose == 0 && any_error == 0 => normal.

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


