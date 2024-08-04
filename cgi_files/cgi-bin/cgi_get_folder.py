#!/usr/bin/env python3
import cgi
import cgitb
import os
import html

cgitb.enable()

print("Content-Type: text/html\n")

def list_directory_contents(directory):
    try:
        items = os.listdir(directory)
        return items
    except FileNotFoundError:
        return None
    except PermissionError:
        return None

form = cgi.FieldStorage()
folder_name = form.getvalue("folder_name", "")

# Sanitize the input to prevent directory traversal
folder_name = os.path.basename(folder_name)

# Set a base directory for security (change this to your desired base path)
base_directory = r"C:\Users\firas\OneDrive\Desktop\folder"
full_path = os.path.join(base_directory, folder_name)

contents = list_directory_contents(full_path) if folder_name else None

html_content = f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Folder Contents</title>
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
        .error {{
            color: red;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>Folder Contents</h1>
        {f'<p>Contents of folder: {html.escape(folder_name)}</p>' if folder_name else ''}
        {
        '<ul>' + ''.join(f'<li>{html.escape(item)}</li>' for item in contents) + '</ul>'
        if contents is not None else
        '<p class="error">Unable to list folder contents. The folder may not exist or you may not have permission to access it.</p>'
        if folder_name else
        '<p>No folder name provided. Please use the form to specify a folder.</p>'
        }
        <br>
        <a href="javascript:history.back()">Go Back</a>
    </div>
</body>
</html>
"""

print(html_content)