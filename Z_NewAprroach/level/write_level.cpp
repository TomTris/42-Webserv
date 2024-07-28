#include "../Tomweb.hpp"

int	writer(Server &server, Connection &cnect, Writer &writer, std::vector<struct pollfd> &fds)
{
	if (check_fds(fds, writer.fdWritingTo) == POLLOUT)
	{
		if (write(writer.fdWritingTo, writer.writeString.c_str(), writer.writeString.length()) == -1)
		{
			std::cout << "write failed" << std::endl;
			cnect.reader.cnect_close = 1;
			return 1;
		}
		std::cout << "write good" << std::endl;
		writer.writeString = "";
	}
	return (1);
}

void	write_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	std::cout << "writer level" << std::endl;
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
