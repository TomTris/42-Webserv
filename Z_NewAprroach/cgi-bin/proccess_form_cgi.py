import os
import urllib.parse
from http.cookies import SimpleCookie

def url_decode(s):
    return urllib.parse.unquote(s)

def delete_spaces(s):
    return s.replace(" ", "")

def check_cookie(data, name):
    cookies = SimpleCookie(os.environ.get("HTTP_COOKIE", ""))
    if name in cookies:
        cookie_value = cookies[name].value
        file_name = name + ".ber"
        try:
            with open(file_name, "r") as file:
                for line in file:
                    if line.startswith(cookie_value + ";"):
                        parts = line.strip().split(";")
                        if len(parts) >= 3:
                            data["name"] = parts[1]
                            data["age"] = parts[2]
                            data["NAME"] = name
                            data["VALUE"] = cookie_value
                            return True
        except FileNotFoundError:
            pass
    return False

def add_to_base(data, name):
    file_name = name + ".ber"
    try:
        number = 0
        with open(file_name, "r") as infile:
            number = sum(1 for _ in infile) + 1
        data["VALUE"] = str(number)
        with open(file_name, "a") as outfile:
            outfile.write(f"{number};{data['name']};{data['age']}\n")
    except FileNotFoundError:
        pass

def parse_form_data(name):
    data = {}
    if check_cookie(data, name):
        return data

    method = os.environ.get("REQUEST_METHOD", "")
    form_data = ""

    if method == "POST":
        form_data = sys.stdin.read()
    elif method == "GET":
        form_data = os.environ.get("QUERY_STRING", "")
    
    if form_data:
        for part in form_data.split("&"):
            if "=" in part:
                key, value = part.split("=", 1)
                data[url_decode(key)] = url_decode(value)
    
    add_to_base(data, name)
    return data

def done(data):
    print(f"Set-Cookie: {data['NAME']}={data['VALUE']}\r\n")
    print("Content-Type: text/html\r\n")
    print("<!DOCTYPE html>")
    print("<html lang=\"en\">")
    print("<head><meta charset=\"UTF-8\"><title>Form Response</title></head>")
    print("<body>")
    print("<h1>Form Data Received</h1>")
    print(f"<p>Name: {data.get('name', '')}</p>")
    print(f"<p>Age: {data.get('age', '')}</p>")
    print("</body>")
    print("</html>")

def main():
    name = "obritt_cgi"
    cookies = os.environ.get("HTTP_COOKIE", "")
    if not cookies or not cookies.startswith(f"{name}="):
        return

    data = parse_form_data(name)
    done(data)

if __name__ == "__main__":
    main()