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
REQUEST_METHOD,
QUERY_STRING,
CONTENT_TYPE,
CONTENT_LENGTH,
HTTP_COOKIE,
SCRIPT_NAME,
REMOTE_ADD,

More than that is not neccessary for this webserver, so it's not included.
In config file, you can change things as you want.

Webserv Configuration
The Webserv configuration is controlled by directives specified in the configuration file. Directives are divided into simple directives and block directives. A simple directive consists of the name and parameters separated by spaces and ends with a semicolon ;. A block directive has a set of additional instructions surrounded by braces { and }.

The File Structure
The webserv configuration file is based on the nginx configuration file structure.

Block directives
http - Provides the configuration file context in which the servers are specified.

server - Sets configuration for a virtual server.

location - Sets configuration depending on a request URI.

http {
    server {
        location [ uri ] {
            [ directives ]
        }
    }
}
Simple directives
listen - Sets the address and port.

server_name - Sets the name of a virtual server.

root - Sets the root directory for requests.

index - Defines files that will be used as an index.

error_page - Defines the URI that will be shown for the specified errors.

client_max_body_size - Sets the maximum allowed size of the client request body.

allow_methods - Sets the HTTP methods to be passed along a request.

autoindex - Enables or disables the directory listing output.

alias - Defines a replacement for the specified location.

cgi - Maps a response file extension to a CGI script.

return - Sends an HTTP response with a specified status code and optional URL.

Documentation
Block Directives
Block directives are used to group multiple directives together. The block directives are: http, server, and location.

http {
    server {
        location [ uri ] {
            [ directives ]
        }
    }
}  
http
Syntax: http { ... }
Default: —
Context: main

Provides the configuration file context in which the servers are specified.

server
Syntax: server { ... }
Default: —
Context: http

Sets configuration for a virtual server.

location
Syntax: location uri { ... }
Default: —
Context: server

Sets configuration depending on a request URI.

Simple Directives
Simple directives are used to specify a single configuration parameter. The simple directives are: listen, server_name, root, index, error_page, client_max_body_size, allow_methods, autoindex, alias, cgi, and return.

http {
    # http directives used as default values
    root                    path;
    index                   file;
    error_page              code uri;
    client_max_body_size    size;
    allow_methods           methods;
    autoindex               [on | off];

    server {
        # server directives
        listen                  address[:port];
        server_name             name;

        root                    path;
        index                   file;
        allow_methods           methods;
        error_page              code uri;
        client_max_body_size    size;
        autoindex               [on | off];
        cgi                     extension script;

        location [ uri ] {
            # directives defined here override the server directives
            index file;
            allow_methods methods;
            error_page code uri;
            client_max_body_size size;

            autoindex [on | off];
            alias path;
            cgi extension script;
            return code URL;
        }
    }
}
listen
Syntax: listen address[:port];
Default: listen *:80;
Context: server

Sets the address and port.
If the port is not specified, the default port is 80.
server_name
Syntax: server_name name;
Default: server_name "";
Context: server

Sets the names of a virtual server.
root
Syntax: root path;
Default: root html;
Context: http, server, location, if in location

Sets the root directory for requests.
index
Syntax: index file;
Default: index index.html;
Context: http, server, location

Defines files that will be used as an index.
error_page
Syntax: error_page code uri;
Default: —
Context: http, server, location, if in location

Defines the URI that will be shown for the specified errors.
Overrides the default error page of the server.
client_max_body_size
Syntax: client_max_body_size size;
Default: client_max_body_size 1m;
Context: http, server, location

Sets the maximum allowed size of the client request body.
If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client.
Sizes can be specified in bytes (e.g., “1024”) kilobytes (e.g., “8k”), or megabytes (e.g., “1m”).
allow_methods
Syntax: allow_methods method ...;
Default: —
Context: http, server, location, if in location

Describes HTTP methods be passed along a request.
The method can be specified as a single method or a list of methods separated by spaces.
GET POST DELETE HEAD
autoindex
Syntax: autoindex [on | off];
Default: autoindex off;
Context: http, server, location

Enables or disables the directory listing output.
Syntax: alias path;
Default: —
Context: location

Defines a replacement for the specified location.
cgi
Syntax: cgi extension script;
Default: —
Context: location

Maps a response file extension to a CGI script.
return
Syntax: return code URL;
Default: —
Context: location

Sends an HTTP response with a specified status code and optional URL.
Directive Contexts
Directive	http	server	location
listen		✓	
server_name		✓	
root	✓	✓	✓
index	✓	✓	✓
error_page	✓	✓	✓
client_max_body_size	✓	✓	✓
allow_methods	✓	✓	✓
autoindex	✓	✓	✓
cgi			✓
return			✓
Thanks for reading.
