#include "Tomweb.hpp"

int	open_file(Connection &current_connection)
{
	std::string file_name;
	int	fd;
	struct stat info;

	if(current_connection.contentType = "data-form")
	{
		file_name = "data";
		fd = open(file_name.c_str(), O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("open");
			return (set_any_error(current_connection, 409, 0, 1));
		}
		current_connection = fd;
	}
	else
	{
		if (stat(file_name.c_str(), &info) == 0)
		{
			set_any_error(current_connection, 409, 0, 1);
		}
		file_name = "z_file/" + current_connection.file_name;
		fd = open(file_name.c_str(), O_CREAT, 0644);
		if (fd == -1)
		{
			perror("open");
			return (set_any_error(current_connection, 500, 0, 1));
		}
		current_connection = fd;
	}
	return (fd);
}

int	body_handle_post(Connection &current_connection)
{
	std::cerr << "handle_post_request called" << std::endl;
	
	if (current_connection.fdWritingTo == -1)
	{
		if (open_file(current_connection) == -1)
			return (-1);
	}
}
