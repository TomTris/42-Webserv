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
	std::cout << "getin 3" << std::endl;
	std::string header = get_header(reader.errNbr);
	std::string file_name = "./www/errors/" + std::to_string(reader.errNbr) + ".html";
	std::cout << file_name << " = file_name" << std::endl;
	int	fd;
	int code;

	std::cout << "---1----" << std::endl;
	if (stat(file_name.c_str(), &info) == -1)
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	std::cout << "---2----" << std::endl;
	if (!S_ISREG(info.st_mode))
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	std::cout << "---3----" << std::endl;
	if (!(info.st_mode & S_IRUSR))
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	std::cout << "---4----" << std::endl;
	fd = open(file_name.c_str(), O_RDONLY);
	if (fd == -1)
	{
		std::cout << "errReturn, open faied" << std::endl;
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	std::cout << "---5----" << std::endl;
	int	length = info.st_size;
	if (length == 0)
		reader.readingDone = 1;
	reader.writer.writeString = get_header(reader.errNbr);
	reader.writer.writeString += "Content-Length: ";
	reader.writer.writeString += std::to_string(length);
	std::cout << "file_length = " << std::to_string(length) << std::endl;
	reader.writer.writeString += "\r\n\r\n";
	reader.openFile = 1;
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;
	add_to_poll(fds, fd, POLLIN);
	change_option_poll(fds, cnect.socket_fd, POLLOUT);
	std::cout << "reader: writer.fdWritingTo: " << reader.writer.fdWritingTo << std::endl;
	return 1;
}

int	openFunc(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	struct stat info;
	int	fd;

	reader.have_read = "";
	if (reader.errNbr < 300)
	{
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
		std::cout << "getiin b" << std::endl;
		if (S_ISDIR(info.st_mode))
		{
			std::cout << "getiin a" << std::endl;
			std::cout << "directory list" << std::endl;
			return (reader.errNbr = 400, openFuncErr(server, cnect, reader, fds));
		}
		if (!S_ISREG(info.st_mode))
			return (reader.errNbr = 403, openFuncErr(server, cnect, reader, fds));
		if (!(info.st_mode & S_IRUSR))
			return (reader.errNbr = 403, openFuncErr(server, cnect, reader, fds));
		fd = open(reader.URI.c_str(), O_RDONLY);
		if (fd == -1)
			return (reader.errNbr = 500, openFuncErr(server, cnect, reader, fds));
		reader.openFile = 1;
		reader.fdReadingFrom = fd;
		reader.writer.fdWritingTo = cnect.socket_fd;
		add_to_poll(fds, reader.fdReadingFrom, POLLIN);
		change_option_poll(fds, cnect.socket_fd, POLLOUT);
		std::cout << "reader: writer.fdWritingTo: " << reader.writer.fdWritingTo << std::endl;
		return (1);
	}
	return (openFuncErr(server, cnect, reader, fds));
}

int	read_write(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	int		check;
	char	buffer[BUFFERSIZE + 1];

	if (check_fds(fds, reader.fdReadingFrom) != POLLIN)
	{
		std::cout << reader.fdReadingFrom <<  " is NOT in POLL IN" << std::endl;
		return 1;
	}
	std::cout << "fd "<< reader.fdReadingFrom<< " IS in POLL IN" << std::endl;
	// std::cout << " reader.fdReadingFrom" <<  reader.fdReadingFrom << std::endl;
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
		std::cout << "fd " << reader.fdReadingFrom << "read done, rm from poll" << std::endl;
		remove_from_poll(reader.fdReadingFrom, fds);
		reader.fdReadingFrom = -1;
		// if (reader.method == "" || reader.method == "GET")
		// 	reader.cnect_close = 1;
		return 1;
	}
	buffer[check] = 0;
	reader.have_read += buffer;
	std::cout << 3001 << std::endl;
	return (1);
}

int	reader_get(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	std::cout << "reader to writer" << std::endl;
	reader.writer.writeString += reader.have_read;
	reader.have_read = "";
	return (1);
}

int	reader(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds, int j)
{
	std::cout << "getin" << std::endl;
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
			std::cout << "reader, j = " << j << std::endl;
			cnect = &servers[i].connections[j];
			if (cnect->readingHeaderDone == 1 && cnect->reader.done == 0)
			{
				reader(servers[i], *cnect, cnect->reader, fds, j);
			}
		}
	}
}
