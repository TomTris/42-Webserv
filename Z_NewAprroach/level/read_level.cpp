#include "../Tomweb.hpp"

int	openFuncErr(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds);
int	anotherErr(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds, int code)
{
	if (reader.errNbr + 50 < code)
	{
		reader.errNbr = code;
		return (openFuncErr(server, cnect, reader, fds));
	}
	cnect.IsAfterResponseClose = 1;
	return (1);
}


int	openFuncErr(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	if (reader.errNbr >= 300 && reader.errNbr < 400)
	{
		reader.writer.writeString = get_header(reader.errNbr, reader.URI);
		// std::cout << 111111 << std::endl;
		// std::cout << 111111 << std::endl;
		// std::cout << 111111 << std::endl;
		// std::cout << 111111 << std::endl;
		// std::cout << 111111 << std::endl;
		// std::cout << 111111 << std::endl;
		// std::cout << 111111 << std::endl;
		// std::cout << 111111 << std::endl;
		// std::cout << 111111 << std::endl;
		
		reader.fdReadingFrom = -1;
		reader.writer.fdWritingTo = cnect.socket_fd;
		change_option_poll(fds, cnect.socket_fd, POLLOUT);
		std::cout << reader.writer.writeString << std::endl;
		reader.readingDone = 1;
		reader.contentLength = 0;
		return (2);
	}

	struct stat info;
	// std::string header = get_header(reader.errNbr);
	std::string file_name = "./www/errors/" + std::to_string(reader.errNbr) + ".html";
	int	fd;
	// std::cout << "reader.method" <<std::endl;
	// std::cout << reader.errNbr << std::endl;
	// std::cout << reader.URI << std::endl;
	reader.method = "GET";
	if (reader.errNbr >= 400)
	{
		reader.have_read = "";
		reader.have_read_2 = "";
		cnect.IsAfterResponseClose = 1;
	}
	if (stat(file_name.c_str(), &info) == -1)
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		// std::cout << "abc" << std::endl;
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	if (!S_ISREG(info.st_mode))
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		{
			// std::cout << "abc1" << std::endl;
			return (anotherErr(server, cnect, reader, fds, 500));
		}
	}
	if (!(info.st_mode & S_IRUSR))
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		// std::cout << "abc1" << std::endl;
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	fd = open(file_name.c_str(), O_RDONLY);
	if (fd == -1)
	{
		if (reader.errNbr == 500)
			return (reader.cnect_close = 1, 1);
		// std::cout << "abc1" << std::endl;
		return (anotherErr(server, cnect, reader, fds, 500));
	}
	int	length = info.st_size;
	if (length == 0)
		reader.readingDone = 1;
	reader.writer.writeString = get_header(reader.errNbr, "0");
	reader.writer.writeString += "Content-Length: ";
	reader.writer.writeString += std::to_string(length);
	reader.writer.writeString += "\r\n\r\n";
	reader.contentLength = length;
	reader.openFile = 1;
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;

	add_to_poll(fds, fd, POLLIN);
	change_option_poll(fds, cnect.socket_fd, POLLOUT);
	return 1;
}

int	post_open(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	struct stat info;
	int			fd;
	
	if (reader.method == "POST" && reader.post == 2)
		return (reader.errNbr = 200, openFuncErr(server, cnect, reader, fds));
	if (*reader.URI.begin() == '/')
		reader.URI.erase(0, reader.URI.find_first_not_of('/'));
	if (stat(reader.URI.c_str(), &info) == 0)
		return (reader.errNbr = 409, openFuncErr(server, cnect, reader, fds));
	switch (errno)
	{
		case EACCES:
			return (reader.errNbr = 401, openFuncErr(server, cnect, reader, fds));
		case ELOOP:
			return (reader.errNbr = 508, openFuncErr(server, cnect, reader, fds));
	}
	fd = open(reader.URI.c_str(), O_CREAT | O_RDWR , 0777);
	if (fd == -1)
	{
		return (reader.errNbr = 500, openFuncErr(server, cnect, reader, fds));
	}
	reader.fdReadingFrom = cnect.socket_fd;
	reader.writer.fdWritingTo = fd;
	reader.openFile = 1;
	
	add_to_poll(fds, fd, POLLOUT);
	change_option_poll(fds, cnect.socket_fd, POLLIN); // can be commented out
	return (1);
}

int	directory_open(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	std::string a;
	std::string b;
	// std::cout << "cnect.socket_fd = "<< cnect.socket_fd << std::endl;
	reader.dir = opendir(reader.URI.c_str());
	if (reader.dir == NULL) {
        return (reader.errNbr = 401, openFuncErr(server, cnect, reader, fds));
    }
	struct dirent *entry;
	// while (1)
	// {
	// 	if (*(reader.URI.end() - 1) == '/')
	// 		reader.URI.erase(reader.URI.length() - 1);
	// 	else
	// 		break ;
	// }
	// reader.URI.append("/", 1);
	reader.writer.writeString = get_header(200, ".html");
	std::string content = "<!DOCTYPE html>\n"
	"<html>\n"
	"	<head>\n"
	"		<title> Directory Listing </title>\n"
	"	</head>\n"
		"<body>\n"
			"<h1>Directory Listing</h1>\n"
			"<ol>\n";
		struct stat	info;
	int i = 0;
    while ((entry = readdir(reader.dir)))
	{
		// std::cout << i++ << std::endl;
		a = "";
		b= "";
		a = reader.URI + entry->d_name;
		stat(a.c_str() , &info);
		b += "<li><a href=\"";
		b += entry->d_name;
		if (S_ISDIR(info.st_mode))
			b += "/";
		b += "\">";
		b += entry->d_name;
		if (S_ISDIR(info.st_mode))
			b += "/";
		b += "</a></li>\n";
		content += b;
    }
		// std::cout << content << std::endl;
		// std::cout << "------------------------" << std::endl;
	// std::cout << 10 << std::endl;
	// sleep (1);
	// std::cout << 11 << std::endl;
	// usleep(40000);
	// std::cout << 12 << std::endl;
	closedir(reader.dir);
	content +=			"<ol>\n"
					"</body>\n"
				"</html>";
	reader.writer.writeString += "Content-Length: " + std::to_string(content.length()) + "\r\n\r\n" + content;
	change_option_poll(fds, cnect.socket_fd, POLLOUT);
	reader.writer.fdWritingTo = cnect.socket_fd;
	// std::cout << "cnect.socket_fd " << cnect.socket_fd << " is put to pollout" << std::endl;
	reader.fdReadingFrom = -1;
	cnect.have_read = reader.have_read;
	reader.have_read_2 = "";
	reader.have_read = "";
	reader.readingDone = 1;
	return (1);
}

int	openFunc(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	struct stat info;
	int	fd;

	if (reader.errNbr >= 300 || reader.errFuncCall == 1)
		return openFuncErr(server, cnect, reader, fds);
	// std::cout << "reader.URI before = " << reader.URI << std::endl;
	if (*reader.URI.begin() == '/')
		reader.URI.erase(0, reader.URI.find_first_not_of('/'));
	reader.URI = "./" + reader.URI;
	// std::cout << "reader.URI after = " << reader.URI << std::endl;
	if (stat(reader.URI.c_str(), &info) == -1)
	{
		switch (errno)
		{
			case ENOENT:
				return (reader.errNbr = 404, openFuncErr(server, cnect, reader, fds));
			case EACCES:
				return (reader.errNbr = 401, openFuncErr(server, cnect, reader, fds));
			case ELOOP:
				return (reader.errNbr = 508, openFuncErr(server, cnect, reader, fds));
			default:
				return (reader.errNbr = 500, openFuncErr(server, cnect, reader, fds));
		}
	}
	// std::cout << 1 << std::endl;
	if (S_ISDIR(info.st_mode))
	{
		// std::cout << 2 << std::endl;
		if (reader.autoIndex == 0)
			return (reader.errNbr = 400, openFuncErr(server, cnect, reader, fds));
		return (directory_open(server, cnect, reader, fds));
	}
	// std::cout << 3 << std::endl;
	if (!S_ISREG(info.st_mode))
		return (reader.errNbr = 403, openFuncErr(server, cnect, reader, fds));
	if (!(info.st_mode & S_IRUSR))
		return (reader.errNbr = 403, openFuncErr(server, cnect, reader, fds));
	fd = open(reader.URI.c_str(), O_RDONLY);
	if (fd == -1)
	{
		return (reader.errNbr = 500, openFuncErr(server, cnect, reader, fds));
	}
	// std::cout << "fd ok" << " URI = " << reader.URI << std::endl;
	int	length = info.st_size;
	if (length == 0)
		reader.readingDone = 1;
	reader.writer.writeString = get_header(reader.errNbr, reader.URI) + \
								"Content-Length: " + std::to_string(length) + "\r\n\r\n";
	reader.contentLength = length;
	reader.openFile = 1;
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;

	add_to_poll(fds, reader.fdReadingFrom, POLLIN);
	change_option_poll(fds, cnect.socket_fd, POLLOUT);
	return (1);
}

int	read_func(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	int		check;
	int 	size;
	int		actual_contentLength;

	actual_contentLength = reader.contentLength;
	if (reader.method == "POST")
		actual_contentLength = reader.contentLength - reader.have_read.length();

	if (actual_contentLength > BUFFERSIZE)
		size = BUFFERSIZE;
	else
		size = actual_contentLength + 1;
	char	buffer[size + 1];

	if ((check_fds(fds, reader.fdReadingFrom) != POLLIN))
		return 1;
	if (reader.writer.writeString.length() != 0)
		return (1);	
	check = read(reader.fdReadingFrom, buffer, sizeof(buffer) - 1);
	// std::cout << "+-+-+-**-+*-+*-+=/*-+=*-+ ??? " << std::endl;

	if (check == -1)
	{
		if (reader.errNbr >= 500)
			return (reader.cnect_close = 1, 1);
		reader.errNbr = 500;
		close(reader.fdReadingFrom);
		remove_from_poll(reader.fdReadingFrom, fds);
		reader.fdReadingFrom = -1;
		reader.openFile = 0;
		return 1;
	}
	if (check == 0)
	{
		reader.readingDone = 1;
		// reader.cnect_close = 1;
		cnect.IsAfterResponseClose = 1;
		return 1;
	}
	// std::string a = "";
	// 	a.append(buffer, check);
	// 	std::cout << a << std::endl;
	// std::cout << "---------------------------\n" << std::endl;
		// sleep(4);
	reader.have_read_2.append(buffer, check);
	return (1);
}

int	reader_get(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	if (reader.dir != NULL)
	{
		reader.dir = NULL;
		return 1;
	}
	if (reader.contentLength <= reader.have_read.length())
	{
		reader.writer.writeString += reader.have_read.substr(0, reader.contentLength);
		reader.have_read.erase(0, reader.contentLength);
		reader.have_read += reader.have_read_2;
		reader.have_read_2 = "";
		cnect.have_read = reader.have_read;
		reader.have_read = "";
		reader.contentLength = 0;
		reader.readingDone = 1;
	}
	else if (reader.contentLength <= reader.have_read_2.length() + reader.have_read.length())
	{
		reader.writer.writeString += reader.have_read;
		reader.have_read = "";
		reader.writer.writeString += reader.have_read_2.substr(0, reader.contentLength - reader.have_read.length());
		reader.have_read_2.erase(0, reader.contentLength - reader.have_read.length());
		cnect.have_read = reader.have_read_2;
		reader.have_read_2 = "";
		reader.contentLength = 0;
		reader.readingDone = 1;
	}
	else
	{
		reader.writer.writeString += reader.have_read;
		reader.writer.writeString += reader.have_read_2;
		reader.contentLength = reader.contentLength - reader.have_read.length() - reader.have_read_2.length();
		reader.have_read = "";
		reader.have_read_2 = "";
	}
	return (1);
}

int	reader_post(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	if (reader.dir != NULL)
		return (reader.dir = NULL, 1);
	if (reader.post == 0)
	{
		if (reader.contentLength <= reader.have_read.length())
		{
			reader.writer.writeString += reader.have_read.substr(0, reader.contentLength);
			reader.have_read.erase(0, reader.contentLength);
			reader.have_read += reader.have_read_2;
			reader.have_read_2 = "";
			cnect.have_read = reader.have_read;
			reader.have_read = "";
			reader.post = 2;
			reader.contentLength = 0;
		}
		else if (reader.contentLength <= reader.have_read_2.length() + reader.have_read.length())
		{
			reader.writer.writeString += reader.have_read;
			reader.have_read = "";
			reader.writer.writeString += reader.have_read_2.substr(0, reader.contentLength - reader.have_read.length());
			reader.have_read_2.erase(0, reader.contentLength - reader.have_read.length());
			cnect.have_read = reader.have_read_2;
			reader.have_read_2 = "";
			reader.post = 2;
			reader.contentLength = 0;
		}
		else
		{
			reader.writer.writeString += reader.have_read;
			reader.writer.writeString += reader.have_read_2;
			reader.contentLength = reader.contentLength - reader.have_read.length() - reader.have_read_2.length();
			reader.have_read = "";
			reader.have_read_2 = "";
		}
		return 1;
	}
	if (reader.post == 2)
	{
		close(reader.writer.fdWritingTo);
		remove_from_poll(reader.writer.fdWritingTo, fds);
		reader.openFile = 0;
		reader.fdReadingFrom = cnect.socket_fd;
		reader.writer.fdWritingTo = -1;
		reader.writer.writeString = "";
		reader.method = "GET";
		reader.errFuncCall = 1;
	}
	return (1);
}

int	isTimeOut(Reader &reader, std::vector<struct pollfd> &fds)
{
	int fd;
	clock_t now = clock();

	if ((now - reader.time_out)/ 1000000 > TIME_OUT
		&& check_fds(fds, reader.fdReadingFrom) != POLLIN)
		return (reader.readingDone = 1, 1);
	return (0);
}

int	handle_delete(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds)
{
	struct stat info;
	int	fd;
	int	check;
	
	reader.method = "GET";

	if (*reader.URI.begin() == '/')
		reader.URI.erase(0, reader.URI.find_first_not_of('/'));
	reader.URI = "./" + reader.URI;
	if (stat(reader.URI.c_str(), &info) == -1)
	{
		switch (errno)
		{
			case ENOENT:
				return (reader.errNbr = 200, reader.errFuncCall = 1, 1);
			case EACCES:
				return (reader.errNbr = 401, 0);
			case ELOOP:
				return (reader.errNbr = 508, 0);
			default:
				return (reader.errNbr = 500, 0);
		}
	}
	if (S_ISDIR(info.st_mode))
		return (reader.errNbr = 400, 0);
	if (!S_ISREG(info.st_mode))
		return (reader.errNbr = 403, 0);
	if (!(info.st_mode & S_IRUSR))
		return (reader.errNbr = 403, 0);
	if (std::remove(reader.URI.c_str()) == 0)
		return (reader.errNbr = 200, reader.errFuncCall = 1, 1);
	else
	{
		return (reader.errNbr = 500, 0);
	}
}

int	reader(Server &server, Connection &cnect, Reader &reader, std::vector<struct pollfd> &fds, int j)
{
	// std::cout << 1111111 << std::endl;
	// std::cout << 1111111 << std::endl;
	// std::cout << 1111111 << std::endl;
	// std::cout << 1111111 << std::endl;
	// std::cout << 1111111 << std::endl;
	std::cout << "reader.errNbr " << reader.errNbr << std::endl;
	if (cnect.reader.method == "DELETE" && reader.errNbr < 300)
		handle_delete(server, cnect, reader, fds);
	else if (isTimeOut(reader, fds) == 1)
		return (1);
	if (reader.openFile == 0)
	{
		if (reader.errNbr >= 300)
		{
			openFuncErr(server, cnect, reader, fds);
			reader.method = "GET";
		}
		else if (reader.method == "GET" || reader.method == "DELETE")
			openFunc(server, cnect, reader, fds);
		else if (reader.method == "POST")
			post_open(server, cnect, reader, fds);
		else
		{
			// std::cout << "reader here 405" << std::endl;
			return (reader.errNbr = 405, 1);
		}
	}
	if (reader.cnect_close == 1 || reader.readingDone == 1)
		return (1);
	read_func(server, cnect, reader, fds);

	if (reader.cnect_close == 1 || reader.readingDone == 1)
		return (1);
	if (reader.method == "GET" || reader.method == "DELETE")
		reader_get(server, cnect, reader, fds);
	else if (reader.method == "POST")
		reader_post(server, cnect, reader, fds);
	return 1;
}

void	read_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	time_t now;
	Connection *cnect;	for (int i = 0; i < servers.size(); i++)
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