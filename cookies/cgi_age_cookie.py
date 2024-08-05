def process_session(sessionID):
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
                break  # Exit loop once we find a match

    # HTML start
    html_output = []
    html_output.append("<html>")
    html_output.append("<head>")
    html_output.append("<title>POST Request Processing</title>")
    html_output.append("</head>")
    html_output.append("<body>")

    # Process and respond to the POST request
    if name and age_str:
        try:
            age = int(age_str)
            html_output.append(f"<h2>Hello, {name}!</h2>")
            html_output.append(f"<p>You are {age} years old.</p>")
        except ValueError:
            html_output.append("<p style='color:red;'>Invalid input. Age must be a number.</p>")
    else:
        html_output.append("<p style='color:red;'>Please enter both name and age.</p>")

    # HTML end
    html_output.append("</body>")
    html_output.append("</html>")

    # Return the complete HTML as a single string
    return "\n".join(html_output)

