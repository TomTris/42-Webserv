#include "../Tomweb.hpp"

int	openFuncErr(Server & server, Connection &cnect, Reader &reader);
int	anotherErr(Server & server, Connection &cnect, Reader &reader, int code)
{
	if (reader.errNbr + 50 < code)
	{
		reader.errNbr = code;
		return (openFuncErr(server, cnect, reader));
	}
	cnect.IsAfterResponseClose = 1;
	return (1);
}
int	open500(Server & server, Connection &cnect, Reader &reader)
{
	if (reader.errNbr == 500)
		return (reader.cnect_close = 1, 1);
	return (anotherErr(server, cnect, reader, 500));
}

int	handle_30x(Connection &cnect, Reader &reader)
{
	reader.writer.writeString = get_header(reader.errNbr, reader.URI);
	if (reader.cookies != 0)
		reader.writer.writeString += "Set-Cookie: sessionId=" + std::to_string(reader.cookies) + "\r\n";
	reader.writer.writeString += "\r\n";
	reader.fdReadingFrom = -1;
	reader.writer.fdWritingTo = cnect.socket_fd;
	change_option_poll(cnect.socket_fd, POLLOUT);
	reader.readingDone = 1;
	reader.contentLength = 0;
	reader.openFile = 1;
	return (2);
}

int	openFuncErr(Server & server, Connection &cnect, Reader &reader)
{
	reader.method = "GET";
	if (reader.errNbr >= 300)
	{
		reader.have_read = "";
		reader.have_read_2 = "";
		reader.writer.writeString = "";
		cnect.IsAfterResponseClose = 1;
	}
	if (reader.errNbr >= 300 && reader.errNbr < 400)
		return (handle_30x(cnect, reader));

	struct stat info;
	int	fd;
	std::string file_name = server.get_error_page(reader.errNbr);
	if (stat(file_name.c_str(), &info) == -1)
		return (open500(server, cnect, reader));
	if (!S_ISREG(info.st_mode))
		return (open500(server, cnect, reader));
	if (!(info.st_mode & S_IRUSR))
		return (open500(server, cnect, reader));
	fd = open(file_name.c_str(), O_RDONLY);
	if (fd == -1)
		return (open500(server, cnect, reader));
	int	length = info.st_size;
	if (length == 0)
		reader.readingDone = 1;
	reader.writer.writeString = get_header(reader.errNbr, "0");
	reader.writer.writeString += "Content-Length: " + std::to_string(length);
	if (reader.cookies != 0)
		reader.writer.writeString += "\r\nSet-Cookie: sessionId=" + std::to_string(reader.cookies);
	reader.writer.writeString += "\r\n\r\n";
	reader.contentLength = length;
	reader.openFile = 1;
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;

	add_to_poll(fd, POLLIN);
	change_option_poll(cnect.socket_fd, POLLOUT);
	return 1;
}

int	post_open(Server & server, Connection &cnect, Reader &reader)
{
	struct stat info;
	int			fd;
	
	if (reader.method == "POST" && reader.post == 2)
		return (reader.errNbr = 200, openFuncErr(server, cnect, reader));
	if (*reader.URI.begin() == '/')
		reader.URI.erase(0, reader.URI.find_first_not_of('/'));
	if (stat(reader.URI.c_str(), &info) == 0)
		return (reader.errNbr = 409, openFuncErr(server, cnect, reader));
	switch (errno)
	{
		case EACCES:
			return (reader.errNbr = 401, openFuncErr(server, cnect, reader));
		case ELOOP:
			return (reader.errNbr = 508, openFuncErr(server, cnect, reader));
	}
	fd = open(reader.URI.c_str(), O_CREAT | O_RDWR , 0777);
	if (fd == -1)
		return (reader.errNbr = 500, openFuncErr(server, cnect, reader));
	reader.fdReadingFrom = cnect.socket_fd;
	reader.writer.fdWritingTo = fd;
	reader.openFile = 1;
	
	add_to_poll(fd, POLLOUT);
	// change_option_poll(cnect.socket_fd, POLLIN); // can be commented out
	return (1);
}

int	directory_open(Server & server, Connection &cnect, Reader &reader)
{
	std::string a;
	std::string b;
	std::string content;
	struct dirent *entry;
	struct stat	info;

	reader.dir = opendir(reader.URI.c_str());
	if (reader.dir == NULL) {
        return (reader.errNbr = 401, openFuncErr(server, cnect, reader));
    }

	reader.writer.writeString = get_header(200, ".html");
	content = "<!DOCTYPE html>\n"
	"<html>\n"
	"	<head>\n"
	"		<title> Directory Listing </title>\n"
	"	</head>\n"
		"<body>\n"
			"<h1>Directory Listing</h1>\n"
			"<ol>\n";
    while ((entry = readdir(reader.dir)))
	{
		a = "";
		b= "";
		a = reader.URI + "/" + entry->d_name;
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
	closedir(reader.dir);
	content +=			"<ol>\n"
					"</body>\n"
				"</html>";
	reader.writer.writeString += "Content-Length: " + std::to_string(content.length()) + "\r\n";
	if (reader.cookies != 0)
		reader.writer.writeString += "Set-Cookie: sessionId=" + std::to_string(reader.cookies) + "\r\n";
	reader.writer.writeString += "\r\n" + content;
	change_option_poll(cnect.socket_fd, POLLOUT);
	reader.writer.fdWritingTo = cnect.socket_fd;
	reader.fdReadingFrom = -1;
	cnect.have_read = reader.have_read;
	reader.have_read_2 = "";
	reader.have_read = "";
	reader.readingDone = 1;
	reader.openFile = 1;
	return (1);
}

int	openFunc(Server & server, Connection &cnect, Reader &reader)
{
	struct stat info;
	int	fd;

	if (reader.errNbr >= 300 || reader.errFuncCall == 1)
		return openFuncErr(server, cnect, reader);
	if (*reader.URI.begin() == '/')
		reader.URI.erase(0, reader.URI.find_first_not_of('/'));
	reader.URI = "./" + reader.URI;
	if (stat(reader.URI.c_str(), &info) == -1)
	{
		switch (errno)
		{
			case ENOENT:
				return (reader.errNbr = 404, openFuncErr(server, cnect, reader));
			case EACCES:
				return (reader.errNbr = 401, openFuncErr(server, cnect, reader));
			case ELOOP:
				return (reader.errNbr = 508, openFuncErr(server, cnect, reader));
			default:
				return (reader.errNbr = 500, openFuncErr(server, cnect, reader));
		}
	}
	if (S_ISDIR(info.st_mode))
	{
		if (reader.autoIndex == 0)
			return (reader.errNbr = 400, openFuncErr(server, cnect, reader));
		return (directory_open(server, cnect, reader));
	}
	if (!S_ISREG(info.st_mode))
		return (reader.errNbr = 403, openFuncErr(server, cnect, reader));
	if (!(info.st_mode & S_IRUSR))
		return (reader.errNbr = 403, openFuncErr(server, cnect, reader));
	fd = open(reader.URI.c_str(), O_RDONLY);
	if (fd == -1)
		return (reader.errNbr = 500, openFuncErr(server, cnect, reader));
	int	length = info.st_size;
	if (length == 0)
		reader.readingDone = 1;
	reader.writer.writeString = get_header(reader.errNbr, reader.URI) + \
								"Content-Length: " + std::to_string(length) + "\r\n";
	if (reader.cookies != 0)
		reader.writer.writeString += "Set-Cookie: sessionId=" + std::to_string(reader.cookies) + "\r\n";
	reader.writer.writeString += "\r\n";
	reader.contentLength = length;
	reader.openFile = 1;
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;

	add_to_poll(reader.fdReadingFrom, POLLIN);
	change_option_poll(cnect.socket_fd, POLLOUT);
	return (1);
}

int	read_func(Connection &cnect, Reader &reader)
{
	int		check;
	int 	size;
	int		actual_contentLength;

	if (!(check_fds(reader.fdReadingFrom) & POLLIN))
		return 1;
	if (reader.writer.writeString.length() != 0)
		return (1);

	actual_contentLength = reader.contentLength;
	if (reader.method == "POST")
		actual_contentLength = reader.contentLength - reader.have_read.length();
	if (actual_contentLength > BUFFERSIZE)
		size = BUFFERSIZE;
	else
		size = actual_contentLength + 1;
	char	buffer[size + 1];
	check = read(reader.fdReadingFrom, buffer, sizeof(buffer) - 1);

	if (check == -1)
	{
		// if (reader.errNbr >= 500)
		// 	return (reader.cnect_close = 1, 1);
		// reader.errNbr = 500;
		// close(reader.fdReadingFrom);
		// remove_from_poll(reader.fdReadingFrom);
		// reader.fdReadingFrom = -1;
		// reader.openFile = 0;
		std::cout << "check in reader = -1 " << std::endl;
		// sleep(1);
		return 1;
	}
	if (check == 0)
		return (reader.readingDone = 1, cnect.IsAfterResponseClose = 1, 1);
	// std::string a = "";
	// a.append(buffer, check);
	// std::cout << a << std::endl;
	reader.have_read_2.append(buffer, check);
	return (1);
}

int	reader_get(Connection &cnect, Reader &reader)
{
	if (reader.dir != NULL)
		return (reader.dir = NULL, 1);
	if (static_cast<unsigned int>(reader.contentLength) <= reader.have_read.length())
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
	else if (static_cast<unsigned int>(reader.contentLength) <= reader.have_read_2.length() + reader.have_read.length())
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

int	reader_post(Connection &cnect, Reader &reader)
{
	if (reader.dir != NULL)
		return (reader.dir = NULL, 1);
	if (reader.post == 0)
	{
		if (static_cast<unsigned int>(reader.contentLength) <= reader.have_read.length())
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
		else if (static_cast<unsigned int>(reader.contentLength) <= reader.have_read_2.length() + reader.have_read.length())
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
		remove_from_poll(reader.writer.fdWritingTo);
		reader.openFile = 0;
		reader.fdReadingFrom = cnect.socket_fd;
		reader.writer.fdWritingTo = -1;
		reader.writer.writeString = "";
		reader.method = "GET";
		reader.errFuncCall = 1;
	}
	return (1);
}


int	handle_delete(Reader &reader)
{
	struct stat info;

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
		return (reader.errNbr = 500, 0);
}
int	isTimeOut(Connection &cnect, Reader &reader)
{
	clock_t now = time(0);

	if (cnect.readingHeaderDone == 1)
	{
		if (difftime(now, reader.time_out) > 10)
			return (cnect.reader.cnect_close = 1, 1);
	}
	return (0);
}

int	reader(Server & server, Connection &cnect, Reader &reader)
{
	if (isTimeOut(cnect, reader) == 1)
		return (1);
	if (cnect.reader.method == "DELETE" && reader.errNbr < 300)
		handle_delete(reader);
	if (reader.openFile == 0)
	{
		if (reader.errNbr >= 300)
		{
			openFuncErr(server, cnect, reader);
			reader.method = "GET";
		}
		else if (reader.method == "GET")
			openFunc(server, cnect, reader);
		else if (reader.method == "POST")
			post_open(server, cnect, reader);
		else
			return (reader.errNbr = 405, 1);
	}
	if (reader.cnect_close == 1 || reader.readingDone == 1)
		return (1);
	read_func(cnect, reader);

	if (reader.cnect_close == 1 || reader.readingDone == 1)
		return (1);
	if (reader.method == "GET" || reader.method == "DELETE")
		reader_get(cnect, reader);
	else if (reader.method == "POST")
		reader_post(cnect, reader);
	return 1;
}

int	child_process(Reader &reader, int fd, std::string URI_name, std::string cookies)
{
	dup2(fd, STDOUT_FILENO);
	setenv("cookies", cookies.c_str(), 1);
	setenv("URL", reader.URI.c_str(), 1);
	setenv("Method", reader.method.c_str(), 1);

	char a1[] = "./a.out";
	char a2[200] = {0};
	(void) URI_name;
	std::strcpy(a2, cookies.c_str());
	char *a[3];
	a[0] = a1;
	a[1] = a2;
	a[2] = NULL;

	execve(a[0], a, NULL);
	perror("execve");
	close(fd);
	exit(EXIT_FAILURE);
}

std::string cookies_find(Reader &reader)
{
	if (reader.cookies == 0)
	{
		Server::cookies_nbr += 1;
		reader.cookies = Server::cookies_nbr;
	}
	return (std::to_string(reader.cookies));
}

int	open_a_fileCGI(Reader &reader)
{
	unsigned int	i = 0;
	std::string		path = "./Qdo/CGI_temp/";
	std::string		cgi_file;
	int	fd;
	std::string	URI_name;
	std::string cookies;
//cookies exists or not
	if (reader.cookies > Server::cookies_nbr)
	{
		std::cerr << "cookies " << reader.cookies << "not exists, > " << Server::cookies_nbr << std::endl;
		return (reader.readCGI = 0, reader.errNbr = 400, -1);
	}
//check if the file exits?
	if (reader.URI.find("&") == std::string::npos)
		URI_name = reader.URI;
	else
		URI_name = reader.URI.substr(0, reader.URI.find("&"));
	if (!access(URI_name.c_str(), F_OK))
		return (reader.readCGI = 0, reader.errNbr = 404, -1);
//find a file to write into, check cookies, set it then.
	while (i < 4294967290)
	{
		cgi_file = path + std::to_string(i);
		if (access(cgi_file.c_str(), F_OK) == -1)
		{
			fd = open(cgi_file.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0777);
			if (fd == -1)
				return (reader.errNbr = 500, reader.readCGI = 0, perror("open"), -1);
			cookies = cookies_find(reader);
			reader.pid = fork();
			if (reader.pid == -1)
				return (close(fd), -1);
			if (reader.pid == 0)
				return (child_process(reader, fd, URI_name, cookies));
			close(fd);
			reader.file_name1 = cgi_file;
			reader.openFile = 1;
			return (1);
		}
		i++;
	}
	return (reader.errNbr = 500, -1);
}

int	readCGIFunc(Reader &reader)
{
	if (reader.openFile == 0)
		if (open_a_fileCGI(reader) == -1)
			return (reader.readCGI = 0, reader.errNbr = 400, 1);
	int	status;

	if (waitpid(reader.pid, &status, WNOHANG) == 0)
		return (1);
	reader.readCGI = 0;
	reader.openFile = 0;
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			reader.errNbr = 200;
			reader.method = "GET";
			reader.URI = reader.file_name1;
			reader.pid = -1;
			return (1);
		}
		reader.errNbr = 400;
		reader.pid = -1;
		return (1);
	}
	reader.errNbr = 500;
	return (1);
}

void	read_level(std::vector<Server> &servers)
{
	Connection *cnect;

	for (unsigned int i = 0; i < servers.size(); i++)
	{
		for (unsigned int j = 0; j < servers[i].connections.size(); j++)
		{
			cnect = &servers[i].connections[j];
			if (cnect->readingHeaderDone == 1 && cnect->reader.readingDone == 0)
			{
				if (cnect->reader.readCGI == 1)
					readCGIFunc(cnect->reader);
				else
					reader(servers[i], *cnect, cnect->reader);
			}
		}
	}
}
