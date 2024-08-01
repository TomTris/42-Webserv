#include "../Tomweb.hpp"

int	writer(Server &server, Connection &cnect, Writer &writer, std::vector<struct pollfd> &fds)
{
	// std::cout <<22222222<< std::endl;
	// std::cout <<22222222<< std::endl;
	// std::cout <<22222222<< std::endl;
	// std::cout <<22222222<< std::endl;
	// std::cout <<22222222<< std::endl;
	if (check_fds(fds, writer.fdWritingTo) == POLLOUT)
	{
		if (writer.writeString.length() > 0)
		{
			// if (cnect.reader.URI.find("ico") == std::string::npos)
				// std::cout << "{" << writer.writeString << "}" << std::endl;
				// sleep(1);
			if (write(writer.fdWritingTo, writer.writeString.c_str(), writer.writeString.length()) == -1)
				return (cnect.reader.cnect_close = 1, 1);
			else
			{
				writer.writeString = "";
				if (cnect.reader.readingDone == 1)
				{
					writer.writingDone = 1;
					// std::cout << "writer.writingDone == " << writer.writingDone << std::endl;
					// std::cout << "cnect.IsAfterResponseClose " << cnect.IsAfterResponseClose << std::endl;
					int fd1 = cnect.socket_fd;
					int	fd2 = cnect.reader.fdReadingFrom;
					int	fd3 = cnect.reader.writer.fdWritingTo;

					if (fd2 != cnect.socket_fd)
					{
						remove_from_poll(fd2, fds);
						close(fd2);
					}
					else if (fd3 != cnect.socket_fd)
					{
						remove_from_poll(fd3, fds);
						close(fd3);
					}
				}
			}
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
