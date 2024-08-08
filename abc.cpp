#include "Z_NewAprroach/Tomweb.hpp"

int	main()
{
	std::string method = "REQUEST_METHOD";
	if (setenv("REQUEST_METHOD", "GET", 1) != 0)
		exit(1);
	char *method1 = getenv("REQUEST_METHOD");
	std::cout << method1 << std::endl;
}