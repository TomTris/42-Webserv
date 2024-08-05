import os
import sys

def is_only_alphabet(s):
    return s.isalpha()

def is_only_digits(s):
    return s.isdigit()

def form1():
    content = """<!DOCTYPE html>
<html>
<head>
    <title>CGI POST Request Example</title>
</head>
<body>
    <h1>Submit Your Information</h1>
    <form method="get" action="/cgi-bin/process_post.py">
        <label for="name">Name:</label>
        <input type="text" id="name" name="name" required>
        <br><br>
        <label for="age">Age:</label>
        <input type="number" id="age" name="age" required>
        <br><br>
        <input type="submit" value="Submit">
    </form>
</body>
</html>"""
    print(content)

def form2(name, age):
    content = f"""<!DOCTYPE html>
<html>
<head>
    <title>Welcome</title>
</head>
<body>
    <h1>Hello {name} {age}</h1>
</body>
</html>"""
    print(content)

def exists_in_database(line):
    parts = line.split(";")
    if len(parts) != 3:
        form1()
        return 2

    name, age_str = parts[1], parts[2]

    if not name or not is_only_alphabet(name):
        form1()
        return 2

    try:
        age = int(age_str)
        if age <= 0:
            form1()
            return 2
    except ValueError:
        form1()
        return 2

    form2(name, age)
    return 0

def new_cookies_no_value():
    form1()
    return 0

def new_cookies_with_value(cookies, url):
    parts = url.split("&")
    if len(parts) != 3:
        form1()
        return 0

    name_part = parts[1]
    age_part = parts[2]

    if "name=" not in name_part or "age=" not in age_part:
        form1()
        return 0

    name = name_part.split("name=")[1]
    age_str = age_part.split("age=")[1]

    if not name or not age_str:
        form1()
        return 0

    if not is_only_alphabet(name) or not is_only_digits(age_str):
        form1()
        return 0

    try:
        age = int(age_str)
    except ValueError:
        form1()
        return 0

    with open("./cookies", "a") as file:
        content = f"{cookies};{name};{age}"
        file.write(content + "\n")

    form2(name, age)
    return 0

def main():
    cookies_str = os.getenv("cookies")
    url_str = os.getenv("URL")

    if cookies_str is None or url_str is None:
        form1()
        return 0

    try:
        with open("./cookies", "r") as file:
            lines = file.readlines()
    except FileNotFoundError:
        with open("./cookies", "w") as file:
            pass
        form1()
        return 0

    for line in lines:
        line = line.strip()
        if line.startswith(cookies_str):
            return exists_in_database(line)

    if "&" not in url_str:
        return new_cookies_no_value()

    return new_cookies_with_value(cookies_str, url_str)

if __name__ == "__main__":
    sys.exit(main())
