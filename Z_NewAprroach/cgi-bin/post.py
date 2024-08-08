import os
import sys
from urllib.parse import parse_qs

def get_content_length():
    try:
        return int(os.environ.get('CONTENT_LENGTH', 0))
    except ValueError:
        return 0

def get_form_data(content_length):
    input_data = sys.stdin.read(content_length)
    return parse_qs(input_data)

def validate_form_data(form_data):
    errors = []

    # Get and clean the data
    name = form_data.get('name', [''])[0].strip()
    age_str = form_data.get('age', [''])[0].strip()

    # Validation checks
    if not name:
        errors.append("name not found")
    if not age_str.isdigit():
        errors.append("age is not a number")
    else:
        age = int(age_str)
        if age < 0:
            errors.append("age is not positive")

    return errors, name, age_str

def generate_response(errors, name, age_str):
    if errors:
        response_body = "<html><body><h1>Errors</h1><ul>"
        for error in errors:
            response_body += f"<li>{error}</li>"
        response_body += "</ul></body></html>"
    else:
        response_body = (
            f"<html><body><h1>Data Received</h1>"
            f"<p>Name: {name}</p>"
            f"<p>Age: {age_str}</p>"
            "</body></html>"
        )
    return response_body

def main():
    content_length = get_content_length()
    form_data = get_form_data(content_length)
    errors, name, age_str = validate_form_data(form_data)
    response_body = generate_response(errors, name, age_str)

    print("Content-Type: text/html")  # Added content type header
    print()  # Blank line between headers and content
    print(response_body)

if __name__ == "__main__":
    main()
