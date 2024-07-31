#include "Tomweb.hpp"

std::string handle_200(std::string str)
{
	std::string http = "HTTP/1.1 200 OK\r\n";
	if (str.find(".html") == str.length() - 4)
		return (http += "Content-Type: text/html; charset=UTF-8\r\n", http);
	if (str.find(".ico") == str.length() - 4)
		return (http += "Content-Type: image/x-icon\r\n", http);
	if (str.find(".o") == str.length() - 4)
		return (http += "Content-Type: application/x-object\r\n", http);
	if (str.find(".cpp") == str.length() - 4)
		return (http += "Content-Type: ext/x-c++\r\n", http);
	if (str.find(".html") == str.length() - 4)
		return (http += "Content-Type: text/html; charset=UTF-8\r\n", http);
	if (str.find(".png") == str.length() - 4)
		return (http += "Content-Type: image/png\r\n", http);
	if (str.find(".txt") == str.length() - 4)
		return (http += "Content-Type: text/plain\r\n", http);
	if (str.find(".jpg") == str.length() - 4)
		return (http += "Content-Type: image/jpeg\r\n", http);
	if (str.find(".mp3") == str.length() - 4)
		return (http += "Content-Type: audio/mpeg\r\n", http);
	if (str.find(".doc") == str.length() - 4)
		return (http += "Content-Type: application/msword\r\n", http);
	if (str.length() >= 5)
	{
		if (str.find(".webp") == str.length() - 4)
			return (http += "Content-Type: image/webp\r\n", http);
		if (str.find(".webm") == str.length() - 4)
			return (http += "Content-Type: video/webm\r\n", http);
	}
	if (str.length() >= 4)
	{
		if (str.find(".gif") == str.length() - 4)
			return (http += "Content-Type: image/gif\r\n", http);
		if (str.find(".css") == str.length() - 4)
			return (http += "Content-Type: text/css\r\n", http);
		if (str.find(".svg") == str.length() - 4)
			return (http += "Content-Type: image/svg+xml\r\n", http);
		if (str.find(".wav") == str.length() - 4)
			return (http += "Content-Type: audio/wav\r\n", http);
		if (str.find(".mp4") == str.length() - 4)
			return (http += "Content-Type: video/mp4\r\n", http);
		if (str.find(".avi") == str.length() - 4)
			return (http += "Content-Type: video/x-msvideo\r\n", http);
		if (str.find(".pdf") == str.length() - 4)
			return (http += "Content-Type: application/pdf\r\n", http);
		if (str.find(".zip") == str.length() - 4)
			return (http += "Content-Type: application/zip\r\n", http);
		if (str.find(".rtf") == str.length() - 4)
			return (http += "Content-Type: application/rtf\r\n", http);
	}
	if (str.length() >= 3)
	{
		if (str.find(".md") == str.length() - 4)
			return (http += "Content-Type: text/markdown\r\n", http);
		if (str.find(".js") == str.length() - 4)
			return (http += "Content-Type: text/javascript\r\n", http);
	}
	return (http += "Content-Type: text/html; charset=UTF-8\r\n", http);
}

//Request proccessed successfull
std::string	handle_204(void)
{
	std::string http = 	"HTTP/1.1 204 KO\r\n"
			"Content-Type: text/html\r\n";
	return (http);
}

std::string	handle_400(void)
{
	std::string http = "HTTP/1.1 400 Bad Request\r\n"
			"Content-Type: text/html\r\n";
			// "\r\n<html><body>"
			// "Bad Request\n"
			// "</body></html>";
	return (http);
}

//Unauthorized
std::string	handle_401(void)
{
	std::string http = "HTTP/1.1 401 Unauthorized\r\n"
			"Content-Type: text/html\r\n";

	return (http);
}
//Forbidden
std::string	handle_403(void)
{
	std::string http = "HTTP/1.1 403 KO\r\n"
			"Content-Type: text/html\r\n";

	return (http);
}

//Not found
std::string	handle_404(void){
	std::string http ="HTTP/1.1 404 Not found\r\n"
			"Content-Type: text/html\r\n";

	return (http);
}

std::string	handle_405(void)
{	std::string http = "HTTP/1.1 405 Method not allowed\r\n"
			"Content-Type: text/html\r\n";

	return (http);
}
//Request Timeout
std::string	handle_408(void)
{
	std::string http = "HTTP/1.1 408 Timeout\r\n"
			"Content-Type: text/html\r\n";
	return (http);
}

//Conflict
std::string	handle_409(void)
{
	std::string http = "HTTP/1.1 409 Conflict\r\n"
			"Content-Type: text/html\r\n";
	return (http);
}
//Content-Length required
std::string	handle_411(void)
{
	std::string http = "HTTP/1.1 411 Content-Length required\r\n"
			"Content-Type: text/html\r\n";

	return (http);
}

std::string	handle_431(void)
{	std::string http = "HTTP/1.1 411 Header field too big\r\n"
			"Content-Type: text/html\r\n";

	return (http);
}

std::string	handle_500(void)
{	std::string http = "HTTP/1.1 500 Internal Server Error\r\n"
			"Content-Type: text/html\r\n";

	return (http);
}

std::string	handle_508(void)
{	std::string http = "HTTP/1.1 508 Loop Detected\r\n"
			"Content-Type: text/html\r\n";

	return (http);
}

std::string	get_header(int nbr, std::string str)
{
	// std::cout << "ERROR NBR = "  << nbr << std::endl;
	// if(nbr != 200)
	// 	sleep(1);
	switch(nbr)
	{
		case 200:
			return handle_200(str);	
		case 204:
			return handle_204();

		case 400:
			return handle_400();
		case 401:
			return handle_401();
		case 403:
			return handle_403();
		case 404:
			return handle_404();
		case 405:
			return handle_405();

		case 408:
			return handle_408();
		case 409:
			return handle_409();
		case 411:
			return handle_411();
		case 431:
			return handle_431();
		case 500:
			return handle_500();
	}	
	
	return handle_411();
}