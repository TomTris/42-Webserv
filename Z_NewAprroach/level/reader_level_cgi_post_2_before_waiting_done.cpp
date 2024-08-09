#include "../Tomweb.hpp"

int	child_child_process(Reader &reader, int fdr, int fdw, int *fdp)
{
	close(fdp[1]);
	close(fdr);
	if (dup2(fdw, STDOUT_FILENO) == -1 || dup2(fdp[0], STDIN_FILENO) == -1)
		return (perror("dup2"), exit(EXIT_FAILURE), -9);
	
	std::string aREQUEST_METHOD = "REQUEST_METHOD=" + reader.method;
	std::string aQUERY_STRING = "QUERY_STRING=" + reader.query_string;
	std::string aCONTENT_TYPE = "CONTENT_TYPE=" + reader.content_type;
	std::string aCONTENT_LENGTH = "CONTENT_LENGTH=" + std::to_string(reader.contentLengthCGI);
	std::string aHTTP_COOKIE = "HTTP_COOKIE=" + reader.cookies;
	std::string aSCRIPT_NAME = "SCRIPT_NAME=" + reader.CGI_path;
	std::string aREMOTE_ADD = "REMOTE_ADDR=127.0.0.1";//server name //server port
	char *env[8];
	char env0[aREQUEST_METHOD.size() + 1]; std::strncpy(env0, aREQUEST_METHOD.c_str(), aREQUEST_METHOD.size()); env0[aREQUEST_METHOD.size()] = 0;
	char env1[aQUERY_STRING.size() + 1]; std::strncpy(env1, aQUERY_STRING.c_str(), aQUERY_STRING.size()); env1[aQUERY_STRING.size()] = 0;
	char env2[aCONTENT_TYPE.size() + 1]; std::strncpy(env2, aCONTENT_TYPE.c_str(), aCONTENT_TYPE.size()); env2[aCONTENT_TYPE.size()] = 0;
	char env3[aCONTENT_LENGTH.size() + 1]; std::strncpy(env3, aCONTENT_LENGTH.c_str(), aCONTENT_LENGTH.size()); env3[aCONTENT_LENGTH.size()] = 0;
	char env4[aHTTP_COOKIE.size() + 1]; std::strncpy(env4, aHTTP_COOKIE.c_str(), aHTTP_COOKIE.size()); env4[aHTTP_COOKIE.size()] = 0;
	char env5[aSCRIPT_NAME.size() + 1]; std::strncpy(env5, aSCRIPT_NAME.c_str(), aSCRIPT_NAME.size()); env5[aSCRIPT_NAME.size()] = 0;
	char env6[aREMOTE_ADD.size() + 1]; std::strncpy(env6, aREMOTE_ADD.c_str(), aREMOTE_ADD.size()); env6[aREMOTE_ADD.size()] = 0;
	env[0] = env0;
	env[1] = env1;
	env[2] = env2;
	env[3] = env3;
	env[4] = env4;
	env[5] = env5;
	env[6] = env6;
	env[7] = NULL;

	// reader.CGI_path = "." + reader.CGI_path;
	// char buf[200];
	while (1)
	{
		while (*reader.CGI_path.begin() == '/')
		{
			if (reader.CGI_path.length() > 1)
				reader.CGI_path.erase(reader.CGI_path.begin());
			else
			{
				reader.CGI_path.clear();
				break ;
			}
		}
		if (reader.CGI_path.find("/") != std::string::npos)
		{
			if (chdir(reader.CGI_path.substr(0, reader.CGI_path.find("/")).c_str()) == -1)
				return (perror("chdir"), exit(EXIT_FAILURE), -99);
			reader.CGI_path.erase(0, reader.CGI_path.find("/") + 1);
		}
		else
			break ;
	}
	// //python3 or sh or bla bla

	char *a[4];
	char a0[200];
	char a1[200];
	if (reader.CGI_path.find(".out") != std::string::npos)
	{
		std::strcpy(a0, reader.CGI_path.c_str());
		a[0] = a0;
		a[1] = NULL;
	}
	else
	{
		std::strcpy(a0, reader.cgi_ex.c_str());
		std::strcpy(a1, reader.CGI_path.c_str());
		a[0] = a0;
		a[1] = a1;
		a[2] = NULL;
	}
	// std::cerr << "aHTTP_COOKIE = {" << aHTTP_COOKIE << "}" << std::endl;
	// std::cerr << "reader.cookies = {" << reader.cookies << "}" << std::endl;
	execve(a[0], a, env);
	perror("perror execve");
	close(fdw);
	close(fdp[0]);
	exit(EXIT_FAILURE);
}

int	static_pid(pid_t nbr, int set)
{
	static pid_t ret = -1;
	
	if (set == 0)
		return (ret);
	ret = nbr;
	return (ret);
}

void kill_child_child(int nbr)
{
	pid_t pid = static_pid(0, 0);
	if (pid != -1)
		kill(pid, SIGTERM);
	exit(nbr + 128);
}

int	main_child_process(int fdw, int *fdp, int fdr, Reader &reader)
{
	static_pid(reader.pid, 1);
	signal(SIGTERM, kill_child_child);
	close(fdw);
	close(fdp[0]);

	int		bytes;
	char	buf[BUFFERSIZE];

	while ((bytes = read(fdr, buf, BUFFERSIZE - 1)) != 0)
	{
		buf[bytes] = 0;
		if (write(fdp[1], buf, bytes) == -1)
			break ;
	}
	close(fdp[1]);
	close(fdr);
	int status;
	if (waitpid(reader.pid, &status, 0) != 0)
	{
		if (WIFEXITED(status))
			exit(WIFEXITED(status));
		exit(EXIT_FAILURE);
	}
	perror("write to fdp[1] failed");
	kill_child_child(-127);
	return (-9);
}

int	child_process2(Connection &cnect, Reader &reader)
{
	signal(SIGTERM, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	reader.pid = -1;

	std::string		cgi_file_read = "./Qdo/CGI_temp/-" + std::to_string(cnect.socket_fd);
	std::string		cgi_file_write = "./Qdo/CGI_temp/" + std::to_string(cnect.socket_fd);
	int	fdr = open(cgi_file_read.c_str(), O_RDONLY);
	std::remove(cgi_file_write.c_str());
	int fdw = open(cgi_file_write.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fdr == -1 || fdw == -1)
		return (close(fdr), perror("open"), exit(EXIT_FAILURE), -9);

	int	fdp[2];
	if (pipe(fdp) == -1)
		return (close(fdr), close(fdw), perror("pipe"), exit(EXIT_FAILURE), -9);
	reader.pid = fork();
	if (reader.pid == -1)
		return (close(fdr), close(fdw), perror("fork"), exit(EXIT_FAILURE), -9);
	if (reader.pid == 0)
		child_child_process(reader, fdr, fdw, fdp);
	return (main_child_process(fdw, fdp, fdr, reader));
}

int	child_create2(Connection &cnect, Reader &reader)
{
	std::string		cgi_file;
	std::string		URI_name;

	reader.childCreated = 1;
	reader.pid = fork();
	if (reader.pid == -1)
		return (reader.errNbr = 500, reader.readCGI = 0, perror("fork"), -1);
	if (reader.pid == 0)
		child_process2(cnect, reader);
	return (1);
}


int	CGI_post_wait_open(Connection &cnect, Reader &reader)
{
	if (reader.childCreated == 0)
		if (child_create2(cnect, reader) == -1)
			return (-1);
	int status;

	if (waitpid(reader.pid, &status, WNOHANG) == 0)
		return (0);
	reader.waitingDone = 1;
	reader.pid = -1;
	if (!WIFEXITED(status) || WEXITSTATUS(status) == 0)
		return (reader.errNbr = 500, reader.readCGI = 0, -1);

	std::string		path = "./Qdo/CGI_temp/";
	std::string		cgi_file;
	int				fd;

//find a file to write into
	cgi_file = path + std::to_string(cnect.socket_fd);
	fd = open(cgi_file.c_str(), O_RDONLY);
	if (fd == -1)
		return (reader.errNbr = 500, reader.readCGI = 0, perror("open"), -1);
	reader.fdReadingFrom = fd;
	reader.writer.fdWritingTo = cnect.socket_fd;
	change_option_poll(cnect.socket_fd, POLLOUT);
	add_to_poll(fd, POLLIN);
	reader.post = 4;
	return (1);
}

int	CGI_post_2(Connection &cnect, Reader &reader)
{
	// std::cout << "reader.post = " << reader.post << std::endl;
	if (reader.post == 3)
		CGI_post_wait_open(cnect, reader);
	else if (reader.post == 4)
		CGI_after_waiting(reader);
	return (1);
}

int readCGIFunc(Connection &cnect, Reader &reader)
{
	if (reader.method == "GET")
	{
		// std::cout << "CGI Method GET" << std::endl;
		if (reader.waitingDone == 0)
			return (CGI_get(cnect, reader));
		return (CGI_after_waiting(reader));
	}
	// std::cout << "CGI Method POST" << std::endl;
	if (reader.post < 3)
		return (CGI_post_1(cnect, reader));
	return (CGI_post_2(cnect, reader));
}

// cnect.socket_fd = 1;
// Get request fork,
// Child: dup2output set env "GET", execute.
// Main: after waiting, open "1", read "1" and response to socket.

// Post request:
// => open file -1, read from socket and write to "-1", close "-1"
// => reading done: close(-1), fork,
// Child: set env "POST", dup2 STD_OUT to "1",read from "-1", send it to cgi by the pipe, after reading, remove, "-1", wait until child done, then exit. if is exited by sig -> kill then exit.
// Main: after waiting, read "1" and response to socket.


// After 