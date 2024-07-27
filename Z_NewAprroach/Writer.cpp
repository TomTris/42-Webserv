#include "Writer.hpp"
#include "Tomweb.hpp"

Writer::Writer(int connect_fd) : connect_fd(connect_fd)
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