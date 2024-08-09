import os
import sys
from io import StringIO

# Import your CGI script here
from proccess_form_cgi import main

def test_cgi_script():
    # Step 1: Simulate environment variables
    os.environ['REQUEST_METHOD'] = 'POST'
    os.environ['CONTENT_TYPE'] = 'application/x-www-form-urlencoded'
    os.environ['HTTP_COOKIE'] = 'obritt_cgi=5'

    # Step 2: Simulate form data
    form_data = "name=John+Doe&age=30"
    
    # Step 3: Redirect stdin to simulate form input
    sys.stdin = StringIO(form_data)
    
    # Step 4: Capture the output
    sys.stdout = StringIO()

    # Step 5: Run the CGI script
    main()

    # Step 6: Get the CGI output
    output = sys.stdout.getvalue()
    sys.stdout = sys.__stdout__

    # print("CGI Script Output:\n")
    print(output)

if __name__ == "__main__":
    test_cgi_script()
