#include "../Tomweb.hpp"

int	main()
{
	char *a;
	a = (char *)malloc(2);
	free(a);
	free(a);
	return (1);
}