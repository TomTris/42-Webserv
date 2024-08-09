#include "../Tomweb.hpp"

int	cgi_post_write_to_file(Connection &cnect, Reader &reader)
{
	if (reader.post == 0)
	{
		if (static_cast<unsigned int>(reader.contentLength) <= reader.have_read_2.length())
		{
			reader.writer.writeString += reader.have_read_2.substr(0, reader.contentLength);
			reader.have_read_2.erase(0, reader.contentLength);
			cnect.have_read = reader.have_read_2;
			reader.post = 2;
			reader.contentLength = 0;
		}
		else
		{
			reader.writer.writeString += reader.have_read_2;
			reader.contentLength -= reader.have_read_2.length();
		}
		reader.have_read_2 = "";
	}
	else if (reader.post == 2 && reader.writer.writeString == "")
	{
		close(reader.writer.fdWritingTo);
		remove_from_poll(reader.writer.fdWritingTo);
		reader.openFile = 0;
		reader.writer.fdWritingTo = -1;
		reader.errNbr = 200;
		reader.post = 3;
	}
	if (reader.contentLength != 0 && reader.eof == 1)
	{
		reader.cnect_close = 1;
		close(reader.writer.fdWritingTo);
		remove_from_poll(reader.writer.fdWritingTo);
		reader.writer.fdWritingTo = -1;
	}
	return (1);
}

int	read_func_cgi_post1(Connection &cnect, Reader &reader)
{
	if (!(check_fds(reader.fdReadingFrom) & POLLIN))
		return 1;
	if (reader.writer.writeString.length() != 0)
		return (1);
	
	int		check;
	char	buffer[BUFFERSIZE];
	if (check_fds(reader.fdReadingFrom) != POLLIN)
		return (0);
	check = read(reader.fdReadingFrom, buffer, sizeof(buffer) - 1);
	revents_to_0(reader.fdReadingFrom);
	if (check == -1)
		return (std::cerr << "check in reader = -1 read_func_cgi_post1" << std::endl, reader.cnect_close = 1, 1);
	if (check == 0)
		return (reader.fdReadingFrom = -1, cnect.IsAfterResponseClose = 1, reader.eof = 1, 0);
	reader.have_read_2.append(buffer, check);
	return (1);
}

int	cgi_open_post(Connection &cnect, Reader &reader)
{
	std::string		path = "./Qdo/CGI_temp/-";
	std::string		cgi_file;
	int				fd;

	if (!access(reader.URI.c_str(), F_OK))
		return (reader.readCGI = 0, reader.errNbr = 404, -1);

//find a file to write into
	cgi_file = path + std::to_string(cnect.socket_fd);
	fd = open(cgi_file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
		return (reader.errNbr = 500, reader.readCGI = 0, perror("open"), -1);
	reader.fdReadingFrom = cnect.socket_fd;
	reader.writer.fdWritingTo = fd;
	reader.openFile = 1;
	add_to_poll(fd, POLLOUT);
	reader.time_out = time(0);
	return (1);
}

int	CGI_post_1(Connection &cnect, Reader &reader)
{
	// usleep(200000);
	if (reader.openFile == 0)
	{
		if (cgi_open_post(cnect, reader) == -1)
			return (-1);
	}
	else
	{
		clock_t now = time(0);
		int	revents = check_fds(reader.fdReadingFrom);
		if (difftime(now, reader.time_out) > 10 && revents != POLLIN)
			return (reader.errNbr = 408, reader.readCGI = 0, -1);
		if (revents == POLLIN)
			reader.time_out = time(0);
		read_func_cgi_post1(cnect, reader);
		cgi_post_write_to_file(cnect, reader);
		if (difftime(now, reader.time_out) > 10 && reader.post < 2)
			return (reader.errNbr = 408, reader.readCGI = 0, -1);
	}
	return (1);
}
