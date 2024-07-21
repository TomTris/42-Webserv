#include "Tomweb.hpp"

void read_socket(int &new_socket, std::string &request_content) {
    ssize_t bytes_read;
    char buffer[4096 + 1] = {0};

    while ((bytes_read = read(new_socket, buffer, sizeof(buffer) - 1)) > 0) {
        request_content.append(buffer, bytes_read);
        if (bytes_read < sizeof(buffer) - 1)
			break;
    }
    
    if (bytes_read == -1) {
        throw std::runtime_error("read failed");
    }    
}

int method_find(int &new_socket, std::string &method, std::string &path, std::string &request_content) {
    std::string HTTP_version;

    method = "";
    path = "" ;
    request_content = "";

	read_socket(new_socket, request_content);
    std::cout << "request_content = {" << request_content << "}" << std::endl;
    std::cout << "length = " << request_content.length() << std::endl;

    size_t request_line_end = request_content.find("\r\n");
    if (request_line_end == std::string::npos) {
        std::cerr << "Invalid request: no end of request line" << std::endl;
        return 0;
    }
    std::string request_line = request_content.substr(0, request_line_end);
    std::stringstream socket_stream(request_line);

    if (!(socket_stream >> method >> path >> HTTP_version)) {
        std::cerr << "Invalid request line: " << request_line << std::endl;
        return 0;
    }

    path.erase(0, path.find_first_not_of('/'));
    if (path == "favicon.io")
        return (0);

    // // Optional: Remove headers from request content
    // size_t headers_end = request_content.find("\r\n\r\n");
    // if (headers_end != std::string::npos) {
    //     request_content = request_content.substr(headers_end + 4);
    // }
    return 1;
}