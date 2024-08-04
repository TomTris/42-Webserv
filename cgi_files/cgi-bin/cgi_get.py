#!/usr/bin/env python
import cgi
import os
import sys
import cgitb
cgitb.enable()

# Function to generate debug information
def get_debug_info():
    debug_info = []
    debug_info.append(f"Python version: {sys.version}")
    debug_info.append(f"Current working directory: {os.getcwd()}")
    debug_info.append("Environment variables:")
    for key, value in os.environ.items():
        debug_info.append(f"{key}: {value}")
    return "\n".join(debug_info)

# Define the default directory for listing files
default_directory = "uploads"
list_directory = os.getenv("LIST_DIRECTORY", default_directory)

# Ensure the directory exists
if not os.path.exists(list_directory):
    os.makedirs(list_directory)

# Initialize response message and file list
message = ''
file_list = ""

# Get the list of files in the directory
try:
    files = os.listdir(list_directory)
    message = f"Files in /{list_directory}:"
    file_list = "<ul>"
    for file in files:
        file_list += f"<li>{file}</li>"
    file_list += "</ul>"
except Exception as e:
    message = f"Error listing files: {str(e)}"

# Generate HTML content
html_content = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>File List</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            line-height: 1.6;
            margin: 0;
            padding: 20px;
            background-color: #f4f4f4;
        }}
        .container {{
            max-width: 800px;
            margin: auto;
            background: white;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }}
        h1 {{
            color: #333;
        }}
        ul {{
            list-style-type: none;
            padding: 0;
        }}
        li {{
            background: #f4f4f4;
            margin-bottom: 5px;
            padding: 10px;
            border-radius: 3px;
        }}
        .debug-info {{
            background: #e6e6e6;
            padding: 10px;
            margin-top: 20px;
            border-radius: 3px;
            font-family: monospace;
            white-space: pre-wrap;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>File List</h1>
        <p>{message}</p>
        {file_list}
        <br>
        <a href="/index.html">Home Page</a>
        <div class="debug-info">
            <h2>Debug Information:</h2>
            <pre>{get_debug_info()}</pre>
        </div>
    </div>
</body>
</html>
"""

# Output the HTML content
print("Content-Type: text/html\n")
print(html_content)