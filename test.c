#include "Tomweb.hpp"

int	main(void)
{
	int fd = open("Tomweb.hpp", O_RDONLY);
	char buffer[50] = {0};

	int	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	printf("%d\n", bytes_read);
	
}