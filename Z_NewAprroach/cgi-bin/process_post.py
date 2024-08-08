#!/usr/bin/env python3

import cgi
import cgitb
from urllib.parse import unquote

# Enable debugging
cgitb.enable()

# Function to parse URL-encoded form data
def parse_form_data():
    form = cgi.FieldStorage()
    data = {}
    for key in form.keys():
        value = form.getvalue(key)
        if value:
            data[key] = unquote(value)
    return data

# Output HTTP headers
print("Content-Type: text/html\r\n\r\n")

# Parse form data
data = parse_form_data()

# Generate response
print("<!DOCTYPE html>")
print("<html lang=\"en\">")
print("<head><meta charset=\"UTF-8\"><title>Form Response</title></head>")
print("<body>")
print("<h1>Form Data Received</h1>")
print(f"<p>Name: {data.get('name', 'Not provided')}</p>")
print(f"<p>Age: {data.get('age', 'Not provided')}</p>")
print("</body>")
print("</html>")
