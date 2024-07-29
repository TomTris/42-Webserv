#include "../Tomweb.hpp"

int	writer(Server &server, Connection &cnect, Writer &writer, std::vector<struct pollfd> &fds)
{
	std::cout << "1-----e------" << std::endl;
	if (check_fds(fds, writer.fdWritingTo) == POLLOUT)
	{
		// std::cerr << "Write size " << writer.writeString.length() << std::endl;
		// std::cerr << "content = " << writer.writeString << std::endl;
		// std::cerr << "write to " <<  writer.fdWritingTo << std::endl;
		std::cout << "1-----f------" << std::endl;
		if (writer.writeString.length() > 0)
		{
			if (write(writer.fdWritingTo, writer.writeString.c_str(), writer.writeString.length()) == -1)
			{
				std::cerr << "write failed, write to " << writer.fdWritingTo << ", length = " << writer.writeString.length() << std::endl;
				// cnect.reader.cnect_close = 1;
				// writer.writeString = "";
				return 1;
			}
			// std::cerr << "write good" << std::endl;
			writer.writeString = "";
		}
	}
	// else
	// 	std::cerr << writer.fdWritingTo << "Not in POLLOUT "<< std::endl;
	std::cout << "1-----j------" << std::endl;
	return (1);
}

void	write_level(std::vector<Server> &servers, std::vector<struct pollfd> &fds)
{
	// std::cerr << "writer level" << std::endl;
	Connection *cnect;
	std::cout << "1-----ab------" << std::endl;
	for (int i = 0; i < servers.size(); i++)
	{
		std::cout << "1-----c------" << std::endl;
		for (int j = 0; j < servers[i].connections.size(); j++)
		{
			std::cout << "1-----d------" << std::endl;
			cnect = &servers[i].connections[j];
			if (cnect->readingHeaderDone == 1)
			{
				writer(servers[i], *cnect, (*cnect).reader.writer, fds);
			}
		}
	}
}
