#include "Tomweb.hpp"


void handle_url(std::string& url)
{
    int ptr = 0;
    while (url.substr(ptr).find('/') != std::string::npos)
    {
        ptr += url.substr(ptr).find('/');
        ptr += 1;
        int len = 0;
        while (url.size() > ptr + len && url[ptr + len] == '/')
            len++;
        url.erase(ptr, len);
    }
	if (url[0] != '/')
		url = "/" + url;
}

int	main(void)
{
	std::string asd = "Asdas//asdasd///a";
	handle_url(asd);
	std::cout << asd << std::endl;
}