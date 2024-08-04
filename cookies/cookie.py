import os

def check_cookie_in_env():
    cookie = os.getenv('cookie')
    
    if cookie is None:
        print("cookie environment variable does not exist.")
        return None
    
    if cookie == '':
        print("cookie environment variable exists but is empty.")
        return None
    
    return cookie

def parse_and_append_cookies(cookie_string):
    # Parse the cookie string
    cookie_pairs = cookie_string.split(';')
    cookies = {}
    for pair in cookie_pairs:
        if '=' in pair:
            key, value = pair.split('=', 1)
            cookies[key.strip()] = value.strip()
    
    # Get the session_id and other cookie values
    session_id = cookies.pop('session_id', 'unknown_session')
    remaining_values = [value for key, value in cookies.items()]
    output_file = "cookizzz"
    
    # Prepare the single-line string
    cookie_line = f"{session_id}:" + ":".join(remaining_values)
    
    # Append to file in a single line
    with open(output_file, 'a') as f:
        f.write(f"{cookie_line}\n")
    
    return cookie_line, output_file

# Usage
cookie = check_cookie_in_env()

if cookie:
    parsed_cookies, filename = parse_and_append_cookies(cookie)
    # print(f"Parsed cookies (appended to {filename}):")
else:
    print("Cookie does not exist in the environment.")

#########################################
#########################################
#########################################
