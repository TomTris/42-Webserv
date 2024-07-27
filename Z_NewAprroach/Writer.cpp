#include "Writer.hpp"
#include "Tomweb.hpp"

Writer::Writer(int connect_fd, std::string &to_write) : connect_fd(connect_fd), writeString(to_write)
{
	fdWritingTo = -1;
	done = 1;
}

void Writer::reset()
{
	done = 1;
	if (fdWritingTo != this->connect_fd && fdWritingTo != -1)
		close(fdWritingTo);
	fdWritingTo = -1;
}