#include "../Tomweb.hpp"

int	writer(Connection &cnect, Writer &writer)
{
	if (check_fds(writer.fdWritingTo) & POLLOUT)
	{
		if (writer.writeString.length() > 0)
		{
			ssize_t bytesWritten = write(writer.fdWritingTo, writer.writeString.c_str(), writer.writeString.length());
			if (bytesWritten == -1)
			{
				std::cerr << "Error writing to file descriptor: " << std::strerror(errno) << std::endl;
				// sleep(1);
				return (1);
				// return (cnect.reader.cnect_close = 1, 1);
			}
			else
			{
				writer.writeString = "";
				if (cnect.reader.readingDone == 1)
				{
					std::cout << cnect.socket_fd << std::endl;
					std::cout << "123" << std::endl;
					writer.writingDone = 1;
					int	fd2 = cnect.reader.fdReadingFrom;
					int	fd3 = cnect.reader.writer.fdWritingTo;
					if (fd2 != cnect.socket_fd && fd2 != -1)
					{
						remove_from_poll(fd2);
						close(fd2);
					}
					else if (fd3 != cnect.socket_fd && fd3 != -1)
					{
						remove_from_poll(fd3);
						close(fd3);
					}
					cnect.reader.fdReadingFrom = -1;
					writer.fdWritingTo = -1;
				}
			}
		}
	}
	return (1);
}

void	write_level(std::vector<Server> &servers)
{
	Connection *cnect;
	// std::cout << "a1-------------" << std::endl;
	for (unsigned int i = 0; i < servers.size(); i++)
	{
		// std::cout << "a2-------------" << std::endl;
		for (unsigned int j = 0; j < servers[i].connections.size(); j++)
		{
			// std::cout << "a3-------------" << std::endl;
			cnect = &servers[i].connections[j];
			if (cnect->readingHeaderDone == 1)
			{
				writer(*cnect, (*cnect).reader.writer);
			}
		}
	}
}
