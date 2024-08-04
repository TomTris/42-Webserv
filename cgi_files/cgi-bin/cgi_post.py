#!/usr/bin/env python

import cgi, os
import cgitb; cgitb.enable()

# Please change the upload dir if the default will be set
upload_directory = "C:\\Users\\firas\\OneDrive\\Desktop\\folder"

# Define the default upload directory
default_upload_directory = "uploads"
# set a dir or default it
upload_directory = os.getenv("UPLOAD_DIRECTORY", default_upload_directory)

# Ensure the upload directory exists or created it if it not persent
if not os.path.exists(upload_directory):
    os.makedirs(upload_directory)

# Initialize response header
header = ''

# Check if the request is using multipart/form-data for file uploads
if 'CONTENT_TYPE' not in os.environ or 'multipart/form-data' not in os.environ['CONTENT_TYPE']:
    header = "Status: 415 Unsupported Media Type\r\nContent-Type: text/html\r\n\r\n"
else:
    form = cgi.FieldStorage()

    # Check if any files were uploaded
    if not form.list:
        header = "Status: 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"
    else:
        # Retrieve the uploaded file
        fileitem = form['filename']

        # Verify if a file was indeed uploaded
        if fileitem.filename:
            # Get the base filename to prevent directory traversal
            filename = os.path.basename(fileitem.filename)
            upload_path = os.path.join(upload_directory, filename)

            # Check if the file already exists
            if not os.path.exists(upload_path):
                # Save the uploaded file to the server
                with open(upload_path, 'wb') as new_file:
                    new_file.write(fileitem.file.read())

                header = "Status: 201 Created\r\nContent-Type: text/html\r\n\r\n"
            else:
                header = "Status: 409 Conflict\r\nContent-Type: text/html\r\n\r\n"
        else:
            header = "Status: 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"

html_content = """<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>POST</title>
    <style>
        body {
            font-family: 'Comic Sans MS', cursive, sans-serif;
            background-size: cover;
            color: #000;
            text-align: center;
        }

        .container {
            background: #e0e0e0;
            border: 2px solid #000;
            border-radius: 10px;
            padding: 20px;
            margin: 50px auto;
            width: 80%;
            max-width: 600px;
            box-shadow: 10px 10px 20px rgba(0, 0, 0, 0.5);
        }

        h1 {
            font-size: 2.5em;
            color: #ff6600;
            text-shadow: 2px 2px 5px #000;
            margin-bottom: 10px;
        }

        p {
            font-size: 1.2em;
            color: #333;
            margin-bottom: 20px;
        }

        .upload-btn-wrapper {
            position: relative;
            overflow: hidden;
            display: inline-block;
            margin-bottom: 20px;
            cursor: pointer;
        }

        .btn {
            border: 2px solid #ff6600;
            color: #ff6600;
            background-color: #fff;
            padding: 10px 30px;
            border-radius: 10px;
            font-size: 18px;
            font-weight: bold;
            box-shadow: 2px 2px 5px #000;
        }

        .upload-btn-wrapper input[type=file] {
            font-size: 100px;
            position: absolute;
            left: 0;
            top: 0;
            opacity: 0;
            cursor: pointer;
        }

        .btn-submit {
            font-size: 18px;
            font-weight: bold;
            background-color: #ff6600;
            color: #fff;
            padding: 10px 30px;
            border: 2px solid #ff6600;
            border-radius: 10px;
            cursor: pointer;
            box-shadow: 2px 2px 5px #000;
        }

        .home-btn {
            font-size: 18px;
            background-color: #0099ff;
            color: #fff;
            padding: 10px 30px;
            border: 2px solid #0099ff;
            border-radius: 10px;
            cursor: pointer;
            box-shadow: 2px 2px 5px #000;
        }

        a {
            text-decoration: none;
        }

        a:hover .home-btn {
            background-color: #007acc;
            border-color: #007acc;
        }

        .btn-submit:hover {
            background-color: #cc5200;
            border-color: #cc5200;
        }

        .btn:hover {
            background-color: #ff6600;
            color: #fff;
        }
    </style>
</head>

<body>
    <div class="container">
        <h1>Upload File</h1>
        <p>Select a file from your device and press "Upload"</p>
        <form action="/cgi-bin/cgi_post.py" method="POST" id="uploadForm" enctype="multipart/form-data" onsubmit="return validateForm()">
            <div class="upload-btn-wrapper">
                <button class="btn">Choose file</button>
                <input type="file" name="filename" id="fileInput"/>
            </div>
            <br>
            <div id="message"></div>
            <br>
            <input class="btn-submit" type="submit" value="Upload">
        </form>
        <br>
        <a href="/index.html"><button class="home-btn"><b>Home Page</b></button></a>
    </div>

    <script>
        function validateForm() {
            let fileInput = document.getElementById("fileInput");
            let fileName = fileInput.value;
            
            if (!fileName) {
                alert("Please choose a file to upload");
                return false;
            }
            return true;
        }
    </script>
</body>

</html>"""


print (header + html_content)

exit(256)