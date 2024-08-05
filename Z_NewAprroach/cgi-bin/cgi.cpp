#include "../Tomweb.hpp"

bool isOnlyAlphabet(std::string& str) {
	std::string::const_iterator it;

	if (str.empty())
		return false;
	for (it = str.begin(); it != str.end(); ++it)
	{
		if (!std::isalpha(static_cast<unsigned char>(*it)))
			return false;
	}
	return true;
}

bool isOnlyDigits(const std::string& str)
{
	if (str.empty())
		return false;

	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (!std::isdigit(static_cast<unsigned char>(*it)))
			return false;
	}
	return true;
}


void form1()
{
	std::string content;

	content = "<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
		"<title>CGI POST Request Example</title>\n"
	"</head>\n"
	"<body>\n"
		"<h1>Submit Your Information</h1>\n"
		"<form method=\"get\" action=\"/cgi-bin/process_post.py\">\n"
			"<label for=\"name\">Name:</label>\n"
			"<input type=\"text\" id=\"name\" name=\"name\" required>\n"
			"<br><br>\n"
			"<label for=\"age\">Age:</label>\n"
			"<input type=\"number\" id=\"age\" name=\"age\" required>\n"
			"<br><br>\n"
			"<input type=\"submit\" value=\"Submit\">\n"
		"</form>\n"
	"</body>\n"
	"</html>";
	std::cout << content << std::endl;
}

void	form2(std::string &name, int age)
{
	std::string content;

	content = "<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
		"<title>Welcome</title>\n"
	"</head>\n"
	"<body>\n"
		"<h1> Hello " + name + " " + std::to_string(age) + "</h1>\n"
	"</body>\n"
	"</html>";
	std::cout << content << std::endl;
}

int	exists_in_database(std::string & line)
{
	if (line.find(";") == std::string::npos)
		return(form1(), 2);
	line.erase(0, line.find(";") + 1);
	
	if (line.find(";") == std::string::npos)
		return(form1(), 2);

	std::string name = line.substr(0, line.find(";"));
	if (name.length() == 0 || isOnlyAlphabet(name) == false)
		return(form1(), 2);

	line.erase(0, line.find(";") + 1);
	if (line.find(";") != std::string::npos)
		return(form1(), 2);
	std::stringstream ss_age(line);
	int age;
	ss_age >> age;
	if (age <= 0)
		return(form1(), 2);
	form2(name, age);
	return(0);
}

int new_cookies_no_value(void)
{
	return (form1(), 0);
}

int new_cookies_With_value(std::string cookies, std::string url2)
{
	url2.erase(0, url2.find("&") + 1);
	if (url2.find("name=") == std::string::npos
		|| url2.find("&") == std::string::npos
		|| url2.find("age=") == std::string::npos
		|| url2.find("name=") > url2.find("age="))
		return (form1(), 0);
	std::string name = url2.substr(0, url2.find("&"));
	std::string age_str = url2.substr(url2.find("&") + 1);

	name = name.substr(name.find("name=") + 5);
	age_str = age_str.substr(age_str.find("age=") + 4);

	if (name.length() == 0 || age_str.length() == 0)
		return (form1(), 0);
	if (isOnlyAlphabet(name) == false || isOnlyDigits(age_str) == false)
		return (form1(), 0);
	std::stringstream age_ss(age_str);

	int	age;
	age_ss >> age;
	std::ofstream file("./cookies", std::ios::app);
	if (!file.is_open())
		return (1); // webserv err

	std::string content = cookies + ";" + name + ";" + std::to_string(age);
	file << content << std::endl;
	file.close();
	return (form2(name, age), 0);
}

//0. -> good
//1. -> server err
//2. -> bad, return, set a new cookies.
int	main()
{
	const char *cookies_str = std::getenv("cookies");
	const char *url_str = std::getenv("URL");
	
	if (cookies_str == NULL || url_str == NULL)
		return (form1(), 0); // which maybe never happen

	int fd = open("./cookies", O_RDWR, 0644);
	if (fd == -1)
	{
		fd = open("./cookies", O_CREAT | O_RDWR, 0644);
		return (form1(), 0);
	}
	close(fd);
	std::ifstream file("./cookies");
	if (!file.is_open())
		return (1);
	std::string line = "";
	std::string cookies(cookies_str);
	while (std::getline(file, line))
	{
		if (line.find(cookies) == 0)
		{
			file.close();
			return (exists_in_database(line));
		}
	}
	file.close();
	std::string url(url_str);
	
	if (url.find("&") == std::string::npos)
		return (new_cookies_no_value(), 0);
	return (new_cookies_With_value(cookies, url), 0);
}