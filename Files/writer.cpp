#include "../Tomweb.hpp"

int	connection_write(Server &server, Connection &current_connection, std::vector<int> &to_del)
{
	std::string statusCode;
	if (current_connection.isWriting == 1)
	{
		if (current_connection.method == "POST")
			return (body_handle_post(current_connection));
		else
		{
			if (current_connection.errNbr != 0)
			{
				statusCode = get_status_code(current_connection.errNbr);
				write(current_connection.socket_fd,
					statusCode.c_str(),
					statusCode.length());
				if (current_connection.IsAfterResponseClose == 1)
				{
					close(current_connection.socket_fd);
					to_del.push_back(current_connection.socket_fd);
				}
			}
			else if (current_connection.method == "GET")
			{
				int fd = open(current_connection.URI.c_str(), O_RDONLY);
				char buffer[5000];
				
				if (fd != -1)
				{
					buffer[read(fd, buffer, 4999)] = 0;
					write(current_connection.socket_fd, buffer, strlen(buffer));
					statusCode = get_status_code(200);
					write(fd, statusCode.c_str(), statusCode.length());
					close(fd);
				}
				else
				{
					statusCode = get_status_code(400);
					write(fd, statusCode.c_str(), statusCode.length());
					close(fd);
				}
			}
		}
	}
	return (1);
}