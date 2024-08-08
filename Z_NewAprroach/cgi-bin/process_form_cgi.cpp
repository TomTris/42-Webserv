#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
#include <vector>
#include <sstream> 
#include <fstream>


std::string to_str(long long number)
{
	std::ostringstream oss;

    oss << number;

    std::string str = oss.str();
	return str;
}

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


std::string delete_spaces(std::string str)
{
	std::string out = "";
	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (str[i] != ' ')
			out.push_back(str[i]);
	}
	return out;
}

int check_coockie(std::map<std::string, std::string>& data, std::string& name)
{
	std::string line = "";

	std::string c_name = name + "=";
	char *cookie = getenv("HTTP_COOKIE");
	if (cookie)
	{	
		std::string s_cookie = delete_spaces(cookie);
		if (s_cookie.find(c_name) != std::string::npos && (s_cookie.find(c_name) == 0 || (s_cookie[s_cookie.find(c_name) - 1] == ';')))
		{
			
			size_t start = s_cookie.find(c_name) + c_name.size();
			size_t len = s_cookie.find(';', start);

			
			std::string value = s_cookie.substr(start, len) + ";";
			std::string line = "";
			std::string name_f = name + ".ber";
			if (value.back() != ';')
				value.push_back(';');

			std::ifstream infile(name_f);
			if (!infile.is_open()) {
				return 0;
			}
			while (std::getline(infile, line))
			{
				if (line.empty()) 
					break;
				if (line.find(value) == 0)
				{
					line = line.substr(value.size());
					std::string name_r = line.substr(0, line.find(';')); 
					std::string age = line.substr(line.find(';') + 1);
					data["name"] = name_r;
					data["age"] = age;
					data["NAME"] = name;
					data["VALUE"] = value;
					return 1;
				}
			}
		}
	}
	return (0);
}


void add_to_base(std::map<std::string, std::string>& data, std::string& name)
{
	data["NAME"] = name;
	name += ".ber";
	try 
	{
		long long number = 0;
		std::ifstream infile(name);
		if (infile.is_open()) {
			std::string line = "";
			while (std::getline(infile, line))
			{
				if (line.empty()) 
					break;
				number += 1;
			}
		}
		
		number++;
		data["VALUE"] = to_str(number);
		infile.close();
		
		std::ofstream outfile(name, std::ios::app);
		if (!outfile.is_open()) {
			return ;
		}
		outfile << (to_str(number) + ";" + data["name"] + ";"+ data["age"] + "\n");
		outfile.close();
	}
	catch(const std::exception& e)
	{
		return ;
	}
}

// HTTP_COOKIES= " cgi_age_name=0; warehourse=1"
// HTTP_COOKIES= " cgi_age_name=1; warehourse=2"
// HTTP_COOKIES= " cgi_age_name=2; warehourse=3"
// HTTP_COOKIES= " cgi_age_name=3; warehourse=4"
// HTTP_COOKIES= " cgi_age_name=4; warehourse=5"
std::map<std::string, std::string> parse_form_data(std::string& name)
{
	std::map<std::string, std::string> data;
	std::string line;

	std::string form_data = "";
	char* method = getenv("REQUEST_METHOD");
	char* query = getenv("QUERY_STRING");
	if (method == NULL)
		return data;
	std::string s_method = method;
	if (check_coockie(data, name))
		return data;

	if (s_method == "POST")
	{
		while (std::getline(std::cin, line))
		{
		if (line.empty()) {
				break;
		}
		form_data += line;
	}
	}
	else if (s_method == "GET" && query)
	{
		form_data = query;
	}
	else
		return data;
	
	size_t pos = 0;
	std::string token;
	// std::cout << query << std::endl;
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

	add_to_base(data, name);
	return data;
}


void done(std::map<std::string, std::string>& data)
{
	// todo store the bulshit and say
	std::cout << "Set-Cookie: " + data["NAME"] + "=" + data["VALUE"] + "\r\n";
	std::cout << "Content-Type: text/html\r\n\r\n";
	std::cout << "<!DOCTYPE html>\r\n";
	std::cout << "<html lang=\"en\">\r\n";
	std::cout << "<head><meta charset=\"UTF-8\"><title>Form Response</title></head>\r\n";
	std::cout << "<body>\r\n";
	std::cout << "<h1>Form Data Received</h1>\r\n";
	std::cout << "<p>Name: " << data["name"] << "</p>\r\n";
	std::cout << "<p>Age: " << data["age"] << "</p>\r\n";
	// std::cout << getenv("HTTP_COOKIE") << "\r\n";
	std::cout << "</body>\r\n";
	std::cout << "</html>\r\n";
	exit(0);
}

int main() {

	std::string name = "obritt_cgi";
	char *cookies = getenv("HTTP_COOKIE");
	if (cookies == NULL || cookies[0] == 0)
		exit(1);
	std::string a(cookies);
	if (a.find("./cgi-bin/process_form_cgi.out=8; cgi-bin/process_form_cgi.out=13; obritt_cgi=") != 0)
		exit(1);
	// Parse form data
	std::map<std::string, std::string> data;
	// setenv("QUERY_STRING", "name=qdo&age=1", 1);
	data = parse_form_data(name);
	// std::cerr << data["name"] << ", " << data["age"] << std::endl;
	done(data);

	return 0;
}


//method = GET -> input= QUERY_STRING
//METHOD = POST -> input comes from input.
// you can use a.cpp to chagne value to check. This file is a cpp cgi file
// u can look at it and try to make another in python or any other language
