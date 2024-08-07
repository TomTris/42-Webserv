#include "Tomweb.hpp"

// int	main(int ac, char **av)
// {
// 	std::string ret =
// 	"Content-Type: text/html\r\n\r\n"
//     "<!DOCTYPE html>\n"
//     "<html>\n"
//     "<head>\n"
//     "    <title>CGI Test</title>\n"
//     "</head>\n"
//     "<body>\n"
//     "    <p>HI HI HI HI HIH I CGI TEST</p>\n"
//     "</body>\n"
//     "</html>\n";
// 	std::cout << ret << std::endl;
// 	return (0);
// }

int main(int ac, char **av)
{
    std::string line;
    std::string ret = "Content-Type: text/html\r\n\r\n"
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n";

    while (1)
    {
        std::getline(std::cin, line);
        ret.append(line);
        if (std::cin.eof())
            break ;
        ret.append("\n");
    }
    std::cout << ret;
    std::cout << "</head>\n";
    std::cout << "</html>" << std::ends;
    return (0);
}