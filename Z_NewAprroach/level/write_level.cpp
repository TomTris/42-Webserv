#include "../Tomweb.hpp"

int	writer(Connection &cnect, Writer &writer, std::vector<struct pollfd> &fds)
{
	// std::cout << "a4-------------" << std::endl;
	if (check_fds(fds, writer.fdWritingTo) == POLLOUT)
	{
		// std::cout << "a5-------------" << std::endl;
		if (writer.writeString.length() > 0)
		{
			
			// std::cout << "a6-------------" << std::endl;
			// std::cout << "writer.fdWritingTo = " << std::endl;
			// std::cout << writer.fdWritingTo << std::endl;
			// std::cout << " +++ +++ +++ " << std::endl;
			// std::cout << "writer.writeString = " << writer.writeString << std::endl;
			ssize_t bytesWritten = write(writer.fdWritingTo, writer.writeString.c_str(), writer.writeString.length());
			// std::cout << "bytesWritten = " << bytesWritten << std::endl;
			if (bytesWritten == -1)
			{
				std::cerr << "Error writing to file descriptor: " << std::strerror(errno) << std::endl;
				return (cnect.reader.cnect_close = 1, 1);
			}
			else
			{
				// std::cout << "a7-------------" << std::endl;
				writer.writeString = "";
				if (cnect.reader.readingDone == 1)
				{
					writer.writingDone = 1;
					int	fd2 = cnect.reader.fdReadingFrom;
					int	fd3 = cnect.reader.writer.fdWritingTo;
					if (fd2 != cnect.socket_fd && fd2 != -1)
					{
						remove_from_poll(fd2, fds);
						close(fd2);
					}
					else if (fd3 != cnect.socket_fd && fd3 != -1)
					{
						remove_from_poll(fd3, fds);
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

void	write_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
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
				writer(*cnect, (*cnect).reader.writer, fds);
			}
		}
	}
}
