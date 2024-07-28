#include "../Tomweb.hpp"

int	openFuncErr(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds);
int	anotherErr(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds, int code)
{
	if (reader.errNbr + 50 < code)
	{
		reader.errNbr = code;
		return (openFuncErr(server, cnect, reader, fds));
	}
	reader.cnect_close = 1;
	return (1);
}
int	openFuncErr(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	struct stat info;
	std::string header = get_header(reader.errNbr);
	std::string file_name = "./www/errors/" + std::to_string(reader.errNbr) + ".html";
	int	fd;
	int code;
	
	reader.have_read = "";
	// if (reader.errNbr >= 400)
	// {
	// 	cnect.IsAfterResponseClose = 1;
	// }
	if (stat(file_name.c_str(), &info) == -1)
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	if (!S_ISREG(info.st_mode))
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	if (!(info.st_mode & S_IRUSR))
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	fd = open(file_name.c_str(), O_RDONLY);
	if (fd == -1)
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	int	length = info.st_size;
	if (length == 0)
		reader.readingDone = 1;
	reader.writer.writeString = get_header(reader.errNbr);
	reader.writer.writeString += "Content-Length: ";
	reader.writer.writeString += std::to_string(length);
	reader.writer.writeString += "\r\n\r\n";

	reader.openFile = 1;
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;

	add_to_poll(fds, fd, POLLIN);
	change_option_poll(fds, cnect.socket_fd, POLLOUT);
	return 1;
}

int	openFunc(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	struct stat info;
	int	fd;

	if (reader.errNbr >= 300)
		return openFuncErr(server, cnect, reader, fds);
	if (*reader.URI.begin() == '/')
		reader.URI.erase(0, reader.URI.find_first_not_of('/'));
	reader.URI = "./" + reader.URI;
	if (stat(reader.URI.c_str(), &info) == -1)
	{
		switch (errno)
		{
			case ENOENT:
				return (reader.errNbr = 400, openFuncErr(server, cnect, reader, fds));
			case EACCES:
				return (reader.errNbr = 401, openFuncErr(server, cnect, reader, fds));
			case ELOOP:
				return (reader.errNbr = 508, openFuncErr(server, cnect, reader, fds));
			default:
				return (reader.errNbr = 500, openFuncErr(server, cnect, reader, fds));
		}
	}
	if (S_ISDIR(info.st_mode))
		return (reader.errNbr = 400, openFuncErr(server, cnect, reader, fds));
	if (!S_ISREG(info.st_mode))
		return (reader.errNbr = 403, openFuncErr(server, cnect, reader, fds));
	if (!(info.st_mode & S_IRUSR))
		return (reader.errNbr = 403, openFuncErr(server, cnect, reader, fds));
	fd = open(reader.URI.c_str(), O_RDONLY);
	if (fd == -1)
		return (reader.errNbr = 500, openFuncErr(server, cnect, reader, fds));
	int	length = info.st_size;
	if (length == 0)
		reader.readingDone = 1;
	reader.writer.writeString = get_header(reader.errNbr);
	reader.writer.writeString += "Content-Length: ";
	reader.writer.writeString += std::to_string(length);
	reader.writer.writeString += "\r\n\r\n";

	reader.openFile = 1;
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;

	add_to_poll(fds, reader.fdReadingFrom, POLLIN);
	change_option_poll(fds, cnect.socket_fd, POLLOUT);
	return (1);
}

int	read_write(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	int		check;
	char	buffer[BUFFERSIZE + 1];

	if (check_fds(fds, reader.fdReadingFrom) != POLLIN)
		return 1;
	check = read(reader.fdReadingFrom, buffer, sizeof(buffer) - 1);
	if (check == -1)
	{
		if (reader.errNbr >= 500)
			return (reader.cnect_close = 1, 1);
		reader.errNbr = 500;
		reader.openFile = 0;
		close(reader.fdReadingFrom);
		remove_from_poll(reader.fdReadingFrom, fds);
		reader.fdReadingFrom = -1;
		return 1;
	}
	if (check == 0)
	{
		reader.readingDone = 1;
		close(reader.fdReadingFrom);
		remove_from_poll(reader.fdReadingFrom, fds);
		reader.fdReadingFrom = -1;
		return 1;
	}
	buffer[check] = 0;
	reader.have_read_2 += buffer;
	return (1);
}

int	reader_get(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	reader.writer.writeString += reader.have_read_2;
	reader.have_read_2 = "";
	return (1);
}

int	reader(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds, int j)
{
	if (reader.openFile == 0)
		openFunc(server, cnect, reader, fds);
	read_write(server, cnect, reader, fds);
	if (reader.method == "GET")
		return reader_get(server, cnect, reader, fds);
	// else if (reader.method == "POST")
	// 	return reader_post(server, cnect, reader, fds);
	// else if (reader.method == "DELETE")
	// 	return reader_delete(server, cnect, reader, fds);
	else
	{
		std::cerr << "sth wrong in reader" << std::endl;
	}
	return 1;
}

void	read_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	std::cout << "reader level" << std::endl;
	Connection *cnect;
	for (int i = 0; i < servers.size(); i++)
	{
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			cnect = &servers[i].connections[j];
			if (cnect->readingHeaderDone == 1 && cnect->reader.readingDone == 0)
			{
				reader(servers[i], *cnect, cnect->reader, fds, j);
			}
		}
	}
}
