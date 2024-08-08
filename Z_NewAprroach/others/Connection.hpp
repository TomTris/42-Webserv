#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#pragma once
#include <string.h>
#include <iostream>
#include <dirent.h>
class Writer {
	public:
	
		Writer();
		std::string			writeString;
		int					fdWritingTo;
		int					writingDone;
};

class Reader {
	public:
		Reader();
		Writer								writer;
		int                 				errNbr;
		int									openFile;
		int									readingDone; // reading part done

		int									request_line_done;
		int									contentLength;
		int									autoIndex;

		std::string							method;
		int									post;
		std::string							host;
		std::string							URI;

		int									cnect_close;
		int									fdReadingFrom;

		std::string							have_read_2;
		int									errFuncCall;
		DIR									*dir;
		time_t								time_out;

		int									readCGI;
		int									childCreated;
		int									waitingDone;
		std::string 						cookies;
		pid_t								pid;
		std::string							CGI_path; // script_name
		std::string							query_string;
		std::string							CGI_method;
		std::string							content_type;
		std::string							cgi_ex;
		int									contentLengthCGI;
		int									cgi_header_done;
		int									eof;
};

class Connection {
	public:
        ~Connection();
		Connection(int fd);
        int									socket_fd;
		int									IsAfterResponseClose;
        int									readingHeaderDone;
        std::string							have_read;
		Reader								reader;
		time_t								time_out;
        void reset();
};

#endif

// REQUEST_METHOD: The HTTP method (e.g., GET, POST).
// QUERY_STRING: The query string from the URL (for GET requests).
// CONTENT_TYPE: The MIME type of the request body (for POST requests).
// CONTENT_LENGTH: The length of the request body (for POST requests).
// HTTP_COOKIE: Cookies sent by the client.
// SCRIPT_NAME: The path to the CGI script.
// SERVER_NAME, SERVER_PORT, REMOTE_ADDR, etc.: Server and client information. not handled.
//
