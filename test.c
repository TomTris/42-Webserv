#include "Tomweb.hpp"

int	main(void)
{
	int fd= open("www////200.html", O_RDONLY);
	if (fd > 0)
		std::cout << "Gut" << std::endl;
	else
		std::cout << "bad " << std::endl;
}