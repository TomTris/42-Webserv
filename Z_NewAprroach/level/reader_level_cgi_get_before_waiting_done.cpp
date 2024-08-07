#include "../Tomweb.hpp"

int	after_waiting(Reader &reader, int status)
{
	reader.waitingDone = 1;
	reader.pid = -1;
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		return (1);
	return (reader.errNbr = 500, reader.readCGI = 0, -1);
}

int	wait_done(Reader &reader, int *status)
{
	if (waitpid(reader.pid, status, WNOHANG) == 0)
		return (0);
	return (1);
}

int	cgi_open_get(Connection &cnect, Reader &reader)
{
	std::string		path = "./Qdo/CGI_temp/";
	std::string		cgi_file;
	int				fd;

//find a file to read from
	cgi_file = path + std::to_string(cnect.socket_fd);
	fd = open(cgi_file.c_str(), O_RDONLY);
	if (fd == -1)
		return (reader.errNbr = 500, reader.readCGI = 0, perror("open"), -1);
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;
	change_option_poll(cnect.socket_fd, POLLOUT);
	add_to_poll(fd, POLLIN);
	return (1);
}


int	child_process(Reader &reader, int fd)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	if (setenv("REQUEST_METHOD", reader.method.c_str(), 1) != 0
		|| setenv("QUERY_STRING", reader.query_string.c_str(), 1)
		|| setenv("CONTENT_TYPE", reader.content_type.c_str(), 1)
		|| setenv("CONTENT_LENGTH", "0", 1)
		|| setenv("HTTP_COOKIE", reader.cookies.c_str(), 1)
		|| setenv("SCRIPT_NAME", reader.CGI_path.c_str(), 1)
		|| setenv("REMOTE_ADD", "127.0.0.1", 1))
		exit(EXIT_FAILURE);
		//server name
		//server port
	char a0[reader.CGI_path.length() + 1];
	std::strcpy(a0, reader.CGI_path.c_str());
	char *a[2];
	a[0] = a0;
	a[1] = NULL;
	execve(a0, a, NULL);
	perror("execve");
	close(fd);
	exit(EXIT_FAILURE);
}

int	child_create(Connection &cnect, Reader &reader)
{
	std::string		path = "./Qdo/CGI_temp/";
	std::string		cgi_file;
	int				fd;
	std::string	URI_name;

//check if the cgi_file / cgi_path exits?
	if (!access(URI_name.c_str(), F_OK))
		return (reader.readCGI = 0, reader.errNbr = 404, -1);

//find a file to write into
	cgi_file = path + std::to_string(cnect.socket_fd);
	fd = open(cgi_file.c_str(), O_CREAT | O_RDWR, 0666);
	if (fd == -1)
		return (reader.errNbr = 500, reader.readCGI = 0, perror("open"), -1);
	reader.pid = fork();
	if (reader.pid == -1)
		return (reader.errNbr = 500, reader.readCGI = 0, perror("fork"), -1);
	close(fd);
	if (reader.pid == 0)
		child_process(reader, fd);
	return (1);
}

int CGI_get(Connection &cnect, Reader &reader)
{

	if (reader.childCreated == 0)
		if (child_create(cnect, reader) == -1)
			return (-1);
	if (reader.waitingDone == 0)
	{
		int status;
		if (wait_done(reader, &status) == 0)
			return (1);
		if (after_waiting(reader, status) == -1)
			return (-1);
		if (cgi_open_get(cnect, reader) == -1)
			return (-1);
	}
	return (1);
}
// if (isTimeOut == 1)
	// 	return (cgi_timeout_kill());

// cnect.socket_fd = 1;
// Get request 
// Main: fork, after waiting, open "1", loop (read "1" and response to socket);
// Child: dup2 output to "1", set env "GET", execute.