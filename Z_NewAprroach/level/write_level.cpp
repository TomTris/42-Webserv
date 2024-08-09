#include "../Tomweb.hpp"

int	writer(Connection &cnect, Writer &writer)
{
	int check_fd = check_fds(writer.fdWritingTo);
	if ((check_fd & POLLOUT) && check_fd > 0)
	{
		if (writer.writeString.length() > 0)
		{
			ssize_t bytesWritten = write(writer.fdWritingTo, writer.writeString.c_str(), writer.writeString.length());
			revents_to_0(writer.fdWritingTo);
			if (bytesWritten == -1)
			{
				cnect.reader.cnect_close = 1;
				return (1);
			}
			else
			{
				writer.writeString = "";
				if (cnect.reader.readingDone == 1)
				{
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

	for (unsigned int i = 0; i < servers.size(); i++)
	{
		for (unsigned int j = 0; j < servers[i].connections.size(); j++)
		{
			cnect = &servers[i].connections[j];
			if (cnect->readingHeaderDone == 1)
			{
				writer(*cnect, (*cnect).reader.writer);
			}
		}
	}
}
