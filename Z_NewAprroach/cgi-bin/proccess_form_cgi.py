#!/usr/bin/env python3

import os
import sys
import urllib.parse

def to_str(number):
    return str(number)

def url_decode(encoded_str):
    return urllib.parse.unquote_plus(encoded_str)

def delete_spaces(s):
    return s.replace(" ", "")

def check_cookie(data, name):
    cookie = os.getenv("HTTP_COOKIE")
    if cookie:
        cookie = delete_spaces(cookie)
        cookie_name = name + "="
        if cookie_name in cookie:
            start = cookie.find(cookie_name) + len(cookie_name)
            end = cookie.find(";", start)
            if end == -1:
                end = len(cookie)
            value = cookie[start:end]
            filename = f"{name}.ber"
            try:
                with open(filename, 'r') as f:
                    for line in f:
                        if line.startswith(value):
                            line = line[len(value)+1:]  # Skip past the value and ';'
                            name_r, age = line.split(';', 1)
                            data["name"] = name_r
                            data["age"] = age.strip()
                            data["NAME"] = name
                            data["VALUE"] = value
                            return True
            except FileNotFoundError:
                pass
    return False

def add_to_base(data, name):
    name += ".ber"
    try:
        number = 0
        try:
            with open(name, 'r') as infile:
                for line in infile:
                    number += 1
        except FileNotFoundError:
            pass
        
        number += 1
        data["VALUE"] = to_str(number)
        
        with open(name, 'a') as outfile:
            outfile.write(f"{data['VALUE']};{data['name']};{data['age']}\n")
    except Exception as e:
        pass

def parse_form_data(name):
    data = {}
    form_data = ""

    if check_cookie(data, name):
        return data
    
    method = os.getenv("REQUEST_METHOD", "")
    if method == "POST":
        form_data = sys.stdin.read()
    elif method == "GET":
        form_data = os.getenv("QUERY_STRING", "")
    
    for token in form_data.split('&'):
        if '=' in token:
            key, value = token.split('=', 1)
            key = url_decode(key)
            value = url_decode(value)
            data[key] = value

    if "name" in data and "age" in data:
        add_to_base(data, name)
    
    return data

def done(data):
    print(f"Set-Cookie: {data['NAME']}={data['VALUE']}\r\n", end="")
    print("Content-Type: text/html\r\n\r\n", end="")
    print("<!DOCTYPE html>")
    print("<html lang=\"en\">")
    print("<head><meta charset=\"UTF-8\"><title>Form Response</title></head>")
    print("<body>")
    print("<h1>Form Data Received</h1>")
    print(f"<p>Name: {data['name']}</p>")
    print(f"<p>Age: {data['age']}</p>")
    print("</body>")
    print("</html>")
    sys.exit(0)

if __name__ == "__main__":
    name = "obritt_cgi"
    data = parse_form_data(name)
    done(data)
