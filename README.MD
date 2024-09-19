en.subject.pdf - original description of the Project.
This project is about creating a webserver following HTTP/1.1, even for CGI, in pure C++

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

# Webserv Configuration

The Webserv configuration is controlled by **directives** specified in the configuration file. Directives are divided into simple directives and block directives.
A **simple directive** consists of the name and parameters separated by spaces and ends with a semicolon `;`. A **block directive** has a set of additional instructions surrounded by braces `{` and `}`.

## The File Structure
The webserv configuration file is based on the **nginx** configuration file structure. 


### Block directives

- `http` - Provides the configuration file context in which the servers are specified.   

- `server` - Sets configuration for a virtual server.  

- `location` - Sets configuration depending on a request URI.  

```nginx
server {
    location [ uri ] {
        [ directives ]
    }
}
```


### Simple directives

- `listen` - Sets the address and port.  

- `server_name` - Sets the name of a virtual server.

- `root` - Sets the root directory for requests.

- `index` - Defines files that will be used as an index.

- `error_page` - Defines the URI that will be shown for the specified errors.

- `client_max_body_size` - Sets the maximum allowed size of the client request body.  

- `allow_methods` - Sets the HTTP methods to be passed along a request.

- `autoindex` - Enables or disables the directory listing output.  

- `cgi` - Maps a response file extension to a CGI script.   

- `return` - Sends an HTTP response with a specified status code and optional URL. 


# Documentation

## **Block Directives**

Block directives are used to group multiple directives together. The block directives are: `http`, `server`, and `location`.


```nginx
server {
    location [ uri ] {
        [ directives ]
    }
}
```  

#### `http`

> Syntax: **http** { ... }  
> Default: —  
> Context: main   

Provides the configuration file context in which the servers are specified.   

#### `server`

> Syntax: **server** { ... }  
> Default: —  
> Context: http   

Sets configuration for a virtual server.   

#### `location`

> Syntax: **location** uri { ... }  
> Default: —  
> Context: 	server   

Sets configuration depending on a request URI.   


  
## **Simple Directives**  

Simple directives are used to specify a single configuration parameter. The simple directives are: `listen`, `server_name`, `root`, `index`, `error_page`, `client_max_body_size`, `allow_methods`, `autoindex`, `alias`, `cgi`, and `return`.
server  {
    host    127.0.0.1; #host
    port    8080  ;
    server_name _;
    error_page 404 www/errors/404.html;
    client_max_body_size 10000000;
    location /
    {
        # root /www;
        root /;
        autoindex on;
        
        index  index.html asdas;
        # indexs.html;
        allowed_methods GET POST DELETE;
        cgi_ex .py;
        cgi_ex .sh;
        cgi_path /usr/bin/python3;
        cgi_path /bin/bash;
        
    }
    location /bin-cgi
    {
        cgi_path /usr/bin/python3;
        cgi_path /bin/bash;
        cgi_ex .py .sh;
    } 
    location /bla
    {
        root /qdo;
        index indexs.html index.html;
        allowed_methods POST;
    }

    location /slack
    {
        return https://www.youtube.com/watch?v=nogh434ykF0/;
    }
}


## **Directive Contexts**

>   | **Directive**              |    **http**    |    **server**    |    **location**    |
>   |----------------------------|----------------|------------------|--------------------|
>   | **listen**                 |                |       ✓          |                    |
>   | **server_name**            |                |       ✓          |                    |
>   | **root**                   |       ✓        |       ✓          |       ✓            |
>   | **index**                  |       ✓        |       ✓          |       ✓            |
>   | **error_page**             |       ✓        |       ✓          |       ✓            |
>   | **client_max_body_size**   |       ✓        |       ✓          |       ✓            |
>   | **allow_methods**          |       ✓        |       ✓          |       ✓            |
>   | **autoindex**              |       ✓        |       ✓          |       ✓            |
>   | **cgi**                    |                |                  |       ✓            |
>   | **return**                 |                |                  |       ✓            |
