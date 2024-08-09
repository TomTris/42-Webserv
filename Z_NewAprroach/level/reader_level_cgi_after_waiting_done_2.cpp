#include "../Tomweb.hpp"

std::string	getHeader()
{
	std::string http = "HTTP/1.1 " + std::to_string(200) + " ";
	return (http += "OK\r\n", http);
}

int	extract_contentLength_cgi(std::string &header)
{
	std::string	str = "Content-Length:";
	ssize_t	start = header.find(str);
	if (start == static_cast<ssize_t>(std::string::npos))
		return (-1);
	start += str.length();
	ssize_t end = header.find("\r\n", start);
	std::string content = header.substr(start, end - start);
	std::stringstream socket_stream(content);
	int value;
	socket_stream >> value;
	return (value);
}

int	cgi_handle_file_header(Reader &reader)
{
	if (reader.have_read_2.find("\r\n\r\n") == std::string::npos)
		return (0);
	std::string header = reader.have_read_2.substr(0, reader.have_read_2.find("\r\n\r\n") + 4);
	reader.have_read_2.erase(0, reader.have_read_2.find("\r\n\r\n") + 4);
	std::string str = "Status: ";

	if (header.find(str) == 0)
	{
		int	start = header.find(str);
		start += str.length();
		int	end = header.find("\r\n", start);
		reader.writer.writeString = "HTTP/1.1 " + header.substr(start, end - start + 2);
		header.erase(0, end + 2);
	}
	else
		reader.writer.writeString = getHeader();

	ssize_t header_end = header.find("\r\n\r\n");
	reader.writer.writeString += header.substr(0, header_end + 4);
	header.erase(0, header_end + 4);
	reader.contentLength = extract_contentLength_cgi(header);
	if (reader.contentLength != -1)
	{
		if (static_cast<unsigned int>(reader.contentLength) >= reader.have_read_2.size())
		{
			reader.contentLength -= reader.have_read_2.size();
			reader.writer.writeString += reader.have_read_2;
			reader.have_read_2 = "";
		}
		else
		{
			reader.writer.writeString += reader.have_read_2.substr(0, reader.contentLength);
			reader.have_read_2.erase(0, reader.contentLength);
			reader.contentLength = 0;
		}
	}
	reader.cgi_header_done = 1;
	if (reader.contentLength == 0)
		reader.readingDone = 1;
	return (1);
}

int	cgi_handle_body(Reader &reader)
{
	if (reader.contentLength == 0)
	{
		return (1);
	}
	if (reader.contentLength == -1)
	{
		reader.writer.writeString += reader.have_read_2;
		reader.have_read_2 = "";
		if (reader.fdReadingFrom == -1)
		{
			reader.contentLength = 0;
			reader.readingDone = 1;
			reader.writer.writeString += "\r\n";
		}
		return (1);
	}
	if (static_cast<unsigned int>(reader.contentLength) <= reader.have_read_2.length())
	{
		reader.writer.writeString += reader.have_read_2.substr(0, reader.contentLength);
		reader.contentLength = 0;
		reader.readingDone = 1;
	}
	else
	{
		reader.writer.writeString += reader.have_read_2;
		reader.contentLength -= reader.have_read_2.length();
	}
	reader.have_read_2 = "";
	return (1);
}

int	read_func_cgi_get(Reader &reader)
{
	int check_fd = check_fds(reader.fdReadingFrom);
	if (check_fd <= 0 || !(check_fds(reader.fdReadingFrom) & POLLIN))
		return 1;
	if (reader.writer.writeString.length() != 0)
		return (1);
	
	int		check;
	char	buffer[BUFFERSIZE];
	
	check = read(reader.fdReadingFrom, buffer, sizeof(buffer) - 1);
	revents_to_0(reader.fdReadingFrom);
	if (check == -1)
		return (std::cerr << "check in reader = -1 read_func_cgi_get" << std::endl, reader.cnect_close = 1, 1);
	if (check == 0)
		return (reader.cnect_close = 1, 0);
	reader.have_read_2.append(buffer, check);
	return (1);
}

int	CGI_after_waiting(Reader &reader)
{
	clock_t now = time(0);
	if (difftime(now, reader.time_out) > 15 && check_fds(reader.writer.fdWritingTo) != POLLOUT)
		return (reader.cnect_close = 1, 1);
	if (read_func_cgi_get(reader) == -1)
		return (-1);
	if (reader.cgi_header_done == 0)
		cgi_handle_file_header(reader);
	if (reader.cgi_header_done == 1 && reader.readingDone == 0)
		cgi_handle_body(reader);
	if (reader.fdReadingFrom == -1 && reader.contentLength != 0)
		return (reader.errNbr = 502, reader.readCGI = 0, perror("fork"), -1);
	return (1);
}
