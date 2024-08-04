sessionID = "abc123"

name = ""
age = 0

# Open and read the file
with open('cookizzz', 'r') as file:
    for line in file:
        # Strip leading/trailing whitespace and split the line
        parts = line.strip().split(':')

        # Ensure there are exactly 3 parts and compare sessionID
        if len(parts) == 3 and parts[0] == sessionID:
            name = parts[1]
            age_str = parts[2]
            # print("cookie exists !!!!!valid")
            break  # Exit loop once we find a match

# Output HTML
print("Content-Type: text/html\n")

# HTML start
print("<html>")
print("<head>")
print("<title>POST Request Processing</title>")
print("</head>")
print("<body>")

# Process and respond to the POST request
if name and age_str:
    try:
        age = int(age_str)
        print(f"<h2>Hello, {name}!</h2>")
        print(f"<p>You are {age} years old.</p>")
    except ValueError:
        print("<p style='color:red;'>Invalid input. Age must be a number.</p>")
else:
    print("<p style='color:red;'>Please enter both name and age.</p>")

# HTML end
print("</body>")
print("</html>")
