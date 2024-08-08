#include "../Tomweb.hpp"

std::string	getHeader(int nbr)
{
	std::string http = "HTTP/1.1 " + std::to_string(nbr) + " ";
	switch (nbr)
	{
		case 200:
			return (http += "OK", http);
		case 204:
			return (http += "OK", http);
		case 301:
			return (http += "Moved Permanently", http);
		case 302:
			return (http += "Found", http);
		case 303:
			return (http += "See Other", http);
		case 304:
			return (http += "Not Modified", http);
		case 307:
			return (http += "Temporary Redirect", http);
		case 308:
			return (http += "Permanent Redirect", http);

		case 400:
			return (http += "Bad Request", http);
		case 401:
			return (http += "Unauthorized", http);
		case 403:
			return (http += "Forbidden", http);
		case 404:
			return (http += "Not found", http);
		case 405:
			return (http += "Method not allowed", http);

		case 408:
			return (http += "Request Timeout", http);
		case 409:
			return (http += "Conflict", http);
		case 411:
			return (http += "Content-Length required", http);
		case 413:
			return (http += "Payload Too Large", http);
		case 414:
			return (http += "URI Too Long", http);
		case 500:
			return (http += "Internal Server Error", http);
		case 502:
			return (http += "Bad Gateway Timeout", http);
		case 504:
			return (http += "Gateway Timeout", http);
		case 508:
			return (http += "Loop Detected", http);
	}
	return (http += "Bad Request", http);
}

//Content-Length == -1 => not set
//else => set
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

	int	statusCode;
	std::string header = reader.have_read_2.substr(0, reader.have_read_2.find("\r\n\r\n"));

	if (header.find("Status:") != std::string::npos)
	{
		std::string str = "Status";
		int	start = header.find(str);
		start += str.length();
		int	end = header.find("\r\n", start);
		std::stringstream ss(header.substr(start, end));
		ss >> statusCode;
		reader.writer.writeString = getHeader(statusCode) + "\r\n";
		header.erase(0, end);
	}
	else
		reader.writer.writeString = getHeader(200) + "\r\n";
	reader.contentLength = extract_contentLength_cgi(header);
	if (reader.contentLength != -1)
	{
		reader.contentLength -= (reader.have_read_2.length() - reader.have_read_2.find("\r\n\r\n") - 4);
		if (reader.contentLength < 0)
		{
			if (static_cast<int>(reader.have_read_2.length()) + reader.contentLength >= 0)
				reader.have_read_2 = reader.have_read_2.substr(0, reader.have_read_2.length() + reader.contentLength);
			//else
				// CGi is crazy
			reader.contentLength = 0;
		}
	}
	reader.cgi_header_done = 1;
	return (1);
}

int	cgi_handle_body(Reader &reader)
{
	if (reader.contentLength == 0)
		return (1);
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
	if (!(check_fds(reader.fdReadingFrom) & POLLIN))
		return 1;
	if (reader.writer.writeString.length() != 0)
		return (1);
	
	int		check;
	char	buffer[BUFFERSIZE];
	
	check = read(reader.fdReadingFrom, buffer, sizeof(buffer) - 1);
	if (check == -1)
		return (printf("check in reader = -1 read_func_cgi_get\n"), 1);
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
	if (reader.cgi_header_done == 1)
		cgi_handle_body(reader);
	if (reader.fdReadingFrom == -1 && reader.contentLength != 0)
		return (reader.errNbr = 502, reader.readCGI = 0, perror("fork"), -1);
	return (1);
}
