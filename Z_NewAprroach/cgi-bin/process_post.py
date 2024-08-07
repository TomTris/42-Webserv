#!/usr/bin python3

import cgi
import cgitb


cgitb.enable()

print("Content-Type: text/html\r\n\r\n")

# HTML start
print("<html>")
print("<head>")
print("<title>POST Request Processing</title>")
print("</head>")
print("<body>")

# Retrieve form data
form = cgi.FieldStorage()
name = form.getvalue("name")
age = form.getvalue("age")

# Process and respond to the POST request
if name and age:
    try:
        age = int(age)
        print(f"<h2>Hello, {name}!</h2>")
        print(f"<p>You are {age} years old.</p>")
    except ValueError:
        print("<p style='color:red;'>Invalid input. Age must be a number.</p>")
else:
    print("<p style='color:red;'>Please enter both name and age.</p>")

# HTML end
print("</body>")
print("</html>")