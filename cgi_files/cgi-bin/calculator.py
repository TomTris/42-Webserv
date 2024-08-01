#!/usr/bin python3


import cgi
import cgitb

# Enable debugging
cgitb.enable()

# Print the HTML headers
print("Content-Type: text/html\n")
print("<html>")
print("<head>")
print("<title>Python CGI Script</title>")
print("</head>")
print("<body>")

# HTML form for user input
print("""
    <h1>Simple Calculator</h1>
    <form method="post" action="/cgi-bin/calculator.py">
        <p>
            <label for="num1">Number 1:</label>
            <input type="text" id="num1" name="num1">
        </p>
        <p>
            <label for="num2">Number 2:</label>
            <input type="text" id="num2" name="num2">
        </p>
        <input type="submit" value="Calculate">
    </form>
""")

# Process form data
form = cgi.FieldStorage()
num1 = form.getvalue("num1")
num2 = form.getvalue("num2")

# Check if the inputs are provided
if num1 is not None and num2 is not None:
    try:
        # Convert inputs to floats and calculate the sum
        result = float(num1) + float(num2)
        print(f"<h2>Result: {num1} + {num2} = {result}</h2>")
    except ValueError:
        print("<p style='color:red;'>Invalid input. Please enter valid numbers.</p>")

# Close HTML tags
print("</body>")
print("</html>")
