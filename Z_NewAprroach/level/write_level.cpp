#include "../Tomweb.hpp"

int	writer(Server &server, Connection &cnect, Writer &writer, std::vector<struct pollfd> &fds)
{
	if (check_fds(fds, writer.fdWritingTo) == POLLOUT)
	{
		if (writer.writeString.length() > 0)
		{
			// if (cnect.reader.URI.find("ico") == std::string::npos)
			// 	std::cout << "{" << writer.writeString << "}" << std::endl;
			if (write(writer.fdWritingTo, writer.writeString.c_str(), writer.writeString.length()) == -1)
				return (cnect.reader.cnect_close = 1, 1);
			else
			writer.writeString = "";
		}
	}
	if (cnect.reader.readingDone == 1)
	{
		int fd1 = cnect.socket_fd;
		int	fd2 = cnect.reader.fdReadingFrom;
		int	fd3 = cnect.reader.writer.fdWritingTo;
		
		if (fd2 == fd1)
			fd2 = -1;
		if (fd3 == fd1)
			fd3 = -1;
		remove_from_poll(fd1, fds);
		close(fd1);
		
		if (fd2 != -1)
		{
			remove_from_poll(fd2, fds);
			close(fd2);
		}
		if (fd3 != -1)
		{
			remove_from_poll(fd3, fds);
			close(fd3);
		}
	}
	return (1);
}

void	write_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	Connection *cnect;
	for (int i = 0; i < servers.size(); i++)
	{
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			cnect = &servers[i].connections[j];
			if (cnect->readingHeaderDone == 1)
			{
				writer(servers[i], *cnect, (*cnect).reader.writer, fds);
			}
		}
	}
}
