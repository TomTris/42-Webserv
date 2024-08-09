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


int	child_process(Connection &cnect, Reader &reader)
{
	std::string		path = "./Qdo/CGI_temp/";
	std::string 	cgi_file = path + std::to_string(cnect.socket_fd);
	int				fd;

	signal(SIGPIPE, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	fd = open(cgi_file.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)
		return (reader.errNbr = 500, reader.readCGI = 0, perror("open"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	std::string aREQUEST_METHOD = "REQUEST_METHOD=" + reader.method;
	std::string aQUERY_STRING = "QUERY_STRING=" + reader.query_string;
	std::cerr << "{" << aQUERY_STRING << "}" << std::endl;
	std::string aCONTENT_TYPE = "CONTENT_TYPE=" + reader.content_type;
	std::string aCONTENT_LENGTH = "CONTENT_LENGTH=0";
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
	std::cerr << "execv get " << std::endl;
	execve(a[0], a, env);
	perror("perror execve");
	exit(EXIT_FAILURE);
}

int	child_create(Connection &cnect, Reader &reader)
{
//check if the cgi_file / cgi_path exits?
	if (!access(reader.URI.c_str(), F_OK))
		return (reader.readCGI = 0, reader.errNbr = 404, -1);

//find a file to write into
	// std::cout << "reader.CGI_path = " << reader.CGI_path << std::endl;
	reader.pid = fork();
	if (reader.pid == -1)
		return (reader.errNbr = 500, reader.readCGI = 0, perror("fork"), -1);
	reader.childCreated = 1;
	if (reader.pid == 0)
		child_process(cnect, reader);
	reader.time_out = time(0);
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
		{
			clock_t now = time(0);
			if (difftime(now, reader.time_out) > 5)
			{
				kill(reader.pid, SIGTERM);
				reader.pid = -1;
				reader.time_out = time(0);
				return (reader.errNbr = 504, reader.readCGI = 0, -1);
			}
			return (1);
		}
		if (after_waiting(reader, status) == -1)
			return (-1);
		if (cgi_open_get(cnect, reader) == -1)
			return (-1);
		reader.time_out = time(0);
	}
	return (1);
}
// if (isTimeOut == 1)
	// 	return (cgi_timeout_kill());

// cnect.socket_fd = 1;
// Get request 
// Main: fork, after waiting, open "1", loop (read "1" and response to socket);
// Child: dup2 output to "1", set env "GET", execute.