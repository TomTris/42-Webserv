#!/usr/bin python3

import cgi
import cgitb

# for enabling debugging
cgitb.enable()

# Print the HTML headers
print("Content-Type: text/html\n")

print("<html>")
print("<head>")
print("<title>Change Background Color</title>")

# retrieve form data
form = cgi.FieldStorage()
# Default color is white
color = form.getvalue("color", "white")  

# Apply the chosen background color
print(f"<style>body {{ background-color: {color}; }}</style>")
print("</head>")
print("<body>")

# Output HTML content
print("<h1>Background Color Changer</h1>")
print("<p>Select a color to change the background.</p>")

# Form for user input
print("""
    <form method="post" action="/cgi-bin/change_color.py">
        <label for="color">Enter Color (name or hex):</label>
        <input type="text" id="color" name="color" placeholder="#ffffff or blue">
        <input type="submit" value="Change Color">
    </form>
""")

# Display the chosen color
print(f"<p>Current Background Color: {color}</p>")


print("</body>")
print("</html>")