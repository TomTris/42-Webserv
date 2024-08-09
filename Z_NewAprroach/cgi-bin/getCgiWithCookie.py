#include "Tomweb.hpp"

import os

def main():
	ret = (
		"Content-Type: text/html\r\n\r\n"
		"<!DOCTYPE html>\n"
		"<html>\n"
		"<head>\n"
		"	<title>CGI Test</title>\n"
		"</head>\n"
		"<body>\n"
		f"	<p> Your Cookies is: {os.getenv('HTTP_COOKIE', '')} </p>\n"
		"</body>\n"
		"</html>\n"
	)
	print(ret)

if __name__ == "__main__":
	main()
