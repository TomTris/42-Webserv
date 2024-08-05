import os
from urllib.parse import parse_qs, urlparse

def check_cookie_and_url_in_env():
    # Check for 'cookie' environment variable
    cookie = os.getenv('cookie')
    if cookie is None:
        print("cookie environment variable does not exist.")
        return None
    
    if cookie == '':
        print("cookie environment variable exists but is empty.")
        return None
    
    # Check for 'URL' environment variable
    url = os.getenv('URL')
    if url is None:
        print("URL environment variable does not exist.")
        return None
    
    if url == '':
        print("URL environment variable exists but is empty.")
        return None
    
    return cookie, url

def parse_and_append_cookies(cookie_string):
    # Get environment variables
    url = os.getenv('URL')
    session_id = os.getenv('cookie', 'unknown_session')  # Default to 'unknown_session' if not set
    
    name = None
    age = None

    if url:
        # Check if the URL has a query part after a `&` character
        parts = url.split('&', 1)
        if len(parts) > 1:
            # The first part is the path, the second part is the query string
            query_string = parts[1]
            # Parsing the query string to get the parameters as a dictionary
            params = parse_qs(query_string)
            # Extracting 'name' and 'age' from the parameters
            name = params.get('name', [None])[0]
            age = params.get('age', [None])[0]

    # Construct the output line
    cookie_line = f"{session_id}:{name or ''}:{age or ''}"

    # Append to file in a single line
    output_file = "cookizzz"
    with open(output_file, 'a') as f:
        f.write(f"{cookie_line}\n")
    
    return cookie_line, output_file

# Usage
cookie = check_cookie_and_url_in_env()

if cookie:
    parsed_cookies, filename = parse_and_append_cookies(cookie)
    # print(f"Parsed cookies (appended to {filename}):")
else:
    print("Cookie does not exist in the environment.")

#########################################
#########################################
#########################################
