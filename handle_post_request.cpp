#include "Tomweb.hpp"

int	pre_handling_file(std::string &request_content, std::string &file_name)
{
	//boundary 
	std::string	boundary_delim;
	std::string boundary_prefix = "boundary=";
	ssize_t	pos1 = request_content.find(boundary_prefix) + boundary_prefix.length();
	if (pos1 == std::string::npos)
		return (0);
	ssize_t	pos2 = request_content.find("\r\n", pos1);
	if (pos2 == std::string::npos)
		return (0);
	boundary_delim = "--" + request_content.substr(pos1, pos2 - pos1);
	
	//file name
	std::string content_dispos = "Content-Disposition: form-data; name=\"file\"; filename=\"";
	pos1 = request_content.find(content_dispos) + content_dispos.length();
	if (pos1 == std::string::npos)
		return (0);
	pos2 = request_content.find("\"\r\n", pos1);
	if (pos2 == std::string::npos)
		return (0);
	file_name = "z_files/" + request_content.substr(pos1, pos2 - pos1);
	request_content.erase(0, pos2);

	//content of the file
	pos1 = request_content.find("\r\n\r\n") + 4;
	if (pos1 == std::string::npos)
		return (0);
	request_content.erase(0, pos1);
	pos2 = request_content.find(boundary_delim);
	if (pos2 == std::string::npos)
		return (0);
	request_content.erase(pos2, request_content.find("\0", pos2));
	return (1);
}

void	file_handling(int	&new_socket, std::string &request_content)
{
	int			 	infile_fd;
	struct stat 	info;
	std::string 	file_name;

	(void) new_socket;
	if (pre_handling_file(request_content, file_name) == 0)
	{
		std::cerr << "Wrong form request1" << std::endl;
		handle_unknown_request(new_socket, file_name);
		return ;
	}
	if (stat(file_name.c_str(), &info) == 0)
	{
		//conflict
		std::cerr << "Confict" << std::endl;
		handle_unknown_request(new_socket, file_name);
	}
	else if (errno == ENOENT)
	{
		infile_fd = open(file_name.c_str(), O_CREAT | O_WRONLY, 0644);
		if (infile_fd < 0)
		{
			perror("coudn't open");
			handle_unknown_request(new_socket, file_name);
		}
		else if (!(infile_fd < 0))
		{
			if (write(infile_fd, request_content.c_str(), request_content.length()) == 0)
			{
				perror("write");
				handle_unknown_request(new_socket, file_name);
			}
			else
			{
				handle_200(new_socket, "Success!");
			}
		}
	}
}

void	text_form_handling(int &new_socket, std::string &request_content, std::string &path)
{
	ssize_t pos;

	pos = request_content.find("data=");
	if (pos == std::string::npos)
	{
		std::cerr << "Wrong form request2" << std::endl;
		handle_unknown_request(new_socket, path);
	}
	else
	{
		std::string data = request_content.substr(pos + 5, request_content.end() - request_content.begin());
		std::ofstream file("data.txt", std::ios::app);
		file << data << std::endl;
		file.close();
	}	
}
void	handle_post_request(int	&new_socket, std::string &request_content, std::string &path)
{
	std::cerr << "handle_post_request called" << std::endl;
	if (path == "upload")
	{
		std::cerr << "upload" << std::endl;
		file_handling(new_socket, request_content);
	}
	else if (path == "text_form")
	{
		std::cerr << "text_form" << std::endl;
		text_form_handling(new_socket, request_content, path);
	}
	else
	{
		std::cerr << "non post is called" << std::endl;
		handle_unknown_request(new_socket, path);
	}
}
