This project is about creating a webserver following HTTP/1.1.

For the server itself, it can handle "GET", "POST" and "DELETE" request.
For "GET" request, you can open a browser and click in a file/directory to open it.
For "POST" request, you can use form "z_post_files.html" to post files.
for "DELETE" request, you can us form "z_delete_form.html" to delete a file base on its URL.

For CGI and Cookie, we handle "GET" and "POST" request. The server will read cookie and
pass it to CGI.

For CGI, We handle "GET" and "POST" request and it applied to all files in "/cgi-bin/".
Because of that, it's understandable when you can't not "GET" or "POST" to a .cpp file.

For "POST" request, you can use form "z_cgi_cookies_cpp.html" and "z_cgi_cookie_py.html"
They send corresponding request to /cgi-bin/process_form_cgi.py and /cgi-bin/process_form_py.out
When you use these form, they send "POST" request.

For "GET" request, you can directly go into /cgi-bin/ and click to process_form_cgi.py or .out,
or getCgiWithCookie.py or getCgiWithCookie.out.
infinitiv.out is an infinitiv CGI, so, after timeout, the server will kill that process.

for Cookie and CGI, we try to handle and pass value as a normal CGI of HTTP/1.1, so for Post request,
we will read form socket and write to the child process by using pipe and set variables as needed.
included:
REQUEST_METHOD
QUERY_STRING
CONTENT_TYPE
CONTENT_LENGTH
HTTP_COOKIE
SCRIPT_NAME
REMOTE_ADD

More than that is not neccessary for this webserver, so it's not included.
In config file, you can change things as you want.

Thanks for reading.