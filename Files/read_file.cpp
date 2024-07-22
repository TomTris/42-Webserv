#include "../Tomweb.hpp"

std::string read_file(int &fd)
{
	std::string	ret;
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE + 1];

	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)	
			throw	std::runtime_error("read Failed");
		if (bytes_read == 0)
			return (ret);
		buffer[bytes_read] = 0;
		ret += std::string(buffer);
	}
	return (ret);
}
