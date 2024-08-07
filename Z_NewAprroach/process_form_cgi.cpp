#include <iostream>
#include <string>
#include <map>

// Function to parse the form data
std::map<std::string, std::string> parse_form_data() {
    std::map<std::string, std::string> data;
    std::string line;
    std::getline(std::cin, line); // Read the first line from stdin
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break; // End of form data
        }
        // URL-decode and parse the key=value pairs
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            // URL-decode (replace '%20' with ' ' and so on)
            // This is a simplified version of URL-decoding
            for (size_t i = 0; i < value.length(); ++i) {
                if (value[i] == '+') {
                    value[i] = ' ';
                }
            }
            data[key] = value;
        }
    }
    return data;
}

int main() {
    // Output HTTP headers
    std::cout << "Content-Type: text/html\n\n";

    // Parse form data
    std::map<std::string, std::string> data = parse_form_data();

    // Generate response
    std::cout << "<!DOCTYPE html>\n";
    std::cout << "<html lang=\"en\">\n";
    std::cout << "<head><meta charset=\"UTF-8\"><title>Form Response</title></head>\n";
    std::cout << "<body>\n";
    std::cout << "<h1>Form Data Received</h1>\n";
    std::cout << "<p>Name: " << data["name"] << "</p>\n";
    std::cout << "<p>Age: " << data["age"] << "</p>\n";
    std::cout << "</body>\n";
    std::cout << "</html>\n";

    return 0;
}
