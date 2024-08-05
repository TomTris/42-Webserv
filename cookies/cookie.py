import os
from urllib.parse import parse_qs, urlparse
import cgi_age_cookie

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

def parse_param_check():

    url = os.getenv('URL')
    session_id = os.getenv('cookie', 'unknown_session') 
    
    name = None
    age = None

    if url:
        parts = url.split('&', 1)
        if len(parts) > 1:
            query_string = parts[1]
            params = parse_qs(query_string)
            name = params.get('name', [None])[0]
            age = params.get('age', [None])[0]
    
    return session_id, name, age

#not finished
def check_cookie_in_db():

    session_id, name, age = parse_param_check()

    with open('cookizzz', 'r') as file:

        for line in file:
            # Strip leading/trailing whitespace and split the line
            parts = line.strip().split(':')
            # Ensure there are exactly 3 parts and compare sessionID
            if len(parts) == 3 and parts[0] == session_id:
                cookie_exists = True
                break  # Exit loop once we find a match
            else:
                cookie_exists = False
        # printing rzlt
        if cookie_exists == True:
            print("cookie is found= TRUE!")
        else:
            print("cookie is NOT found= FALSE!")
                
        return cookie_exists

    
def parse_cookies_and_params():
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
    
    return cookie_line

def append_cookie_to_file(cookie_line, output_file="cookizzz"):
    # Append to file in a single line
    with open(output_file, 'a') as f:
        f.write(f"{cookie_line}\n")
    
    return output_file

# Usage
cookie = check_cookie_and_url_in_env()
cookie_found_db = check_cookie_in_db()

if cookie and not cookie_found_db:
    cookie_line = parse_cookies_and_params()
    filename = append_cookie_to_file(cookie_line)
    print("the cookie does not exist in the DB but it has been added!")
    html_response = cgi_age_cookie.process_session("99")
    print(html_response)
else:
    print("the cookie does exist in the DB")
    html_response = cgi_age_cookie.process_session("959")
    print(html_response)

#########################################
#########################################
#########################################

# both files are present here
