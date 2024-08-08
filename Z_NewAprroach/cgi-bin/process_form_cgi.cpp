#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>

// Function to URL-decode a string
std::string url_decode(const std::string &str) {
	std::string result;
	size_t length = str.length();
	for (size_t i = 0; i < length; ++i) {
		if (str[i] == '%') {
			if (i + 2 < length) {
				int value;
				std::istringstream iss(str.substr(i + 1, 2));
				iss >> std::hex >> value;
				result += static_cast<char>(value);
				i += 2;
			}
		} else if (str[i] == '+') {
			result += ' ';
		} else {
			result += str[i];
		}
	}
	return result;
}
	
// Function to parse the form data
std::map<std::string, std::string> parse_form_data() {
	std::map<std::string, std::string> data;
	std::string line;

	std::string form_data;
	char *method = getenv("REQUEST_METHOD");
	char *query = getenv("QUERY_STRING");

	if (method == NULL)
		return data;
	if (!strcmp(method, "POST"))
	{
		while (std::getline(std::cin, line)) {
			if (line.empty()) {
				break;
			}
			form_data += line;
		}
	}
	else if (!strcmp(method, "GET") && query != NULL)
		form_data = (std::string) query;
	else
		return data;
	// Parse form data (key=value pairs)
	size_t pos = 0;
	std::string token;
	while ((pos = form_data.find('&')) != std::string::npos) {
		token = form_data.substr(0, pos);
		size_t eq_pos = token.find('=');
		if (eq_pos != std::string::npos) {
			std::string key = url_decode(token.substr(0, eq_pos));
			std::string value = url_decode(token.substr(eq_pos + 1));
			data[key] = value;
		}
		form_data.erase(0, pos + 1);
	}

	size_t eq_pos = form_data.find('=');
	if (eq_pos != std::string::npos) {
		std::string key = url_decode(form_data.substr(0, eq_pos));
		std::string value = url_decode(form_data.substr(eq_pos + 1));
		data[key] = value;
	}

	return data;
}

int main() {

	// Parse form data
	std::map<std::string, std::string> data;
	// setenv("QUERY_STRING", "name=qdo&age=1", 1);
	data = parse_form_data();
	// std::cerr << data["name"] << ", " << data["age"] << std::endl;
	std::cout << "Content-Type: text/html\r\n\r\n";
	std::cout << "<!DOCTYPE html>\r\n";
	std::cout << "<html lang=\"en\">\r\n";
	std::cout << "<head><meta charset=\"UTF-8\"><title>Form Response</title></head>\r\n";
	std::cout << "<body>\r\n";
	std::cout << "<h1>Form Data Received</h1>\r\n";
	std::cout << "<p>Name: " << data["name"] << "</p>\r\n";
	std::cout << "<p>Age: " << data["age"] << "</p>\r\n";
	std::cout << "</body>\r\n";
	std::cout << "</html>\r\n";

	return 0;
}