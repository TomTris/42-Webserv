#include "Tomweb.hpp"

void	handle_delete_request(int &new_socket, std::string &path)
{
	std::cout << "handle_delete	_request called" << std::endl;
	(void)new_socket;
	(void)path;
	std::string command = "rm " + path;
	int	result = std::system(command.c_str());
	if (result == 0)
		std::cout << "Success" << std::endl;
	else
		std::cout << "Removed failed" << std::endl;
}
