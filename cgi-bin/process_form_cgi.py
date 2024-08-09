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
		cookies = cookie.split(";")
		ind = -1
		for i, c in enumerate(cookies):
			if (c.startswith(cookie_name)):
				cookie = cookies[i] 
				ind = i
				break
		if -1 != ind:
			s, value = cookie.split("=")
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

def get_coockie_val(name):
	cookie = os.getenv("HTTP_COOKIE")
	if (cookie):
		cookie = delete_spaces(cookie)
		cookies = cookie.split(";")
		cookie_name = name + "="
		ind = -1
		for i, c in enumerate(cookies):
			if (c.startswith(cookie_name)):
				cookie = cookies[i] 
				ind = i
				break
		if ind == -1:
			return ""
		s, value = cookie.split("=")
		return value
	return ""

def add_to_base(data, name):
	fname = name + ".ber"
	data["NAME"] = name
	try:
		value = get_coockie_val(name)
		number = -1
		out = ""
		last = ""
		try:
			with open(fname, 'r') as infile:
				for i, line in enumerate(infile):
					if line.startswith(value) and value != "" and number == -1:
						number = i
						line = f'{value};{data["name"]};{data["age"]}'
					out += line + "\n"
					last = line.split(";")[0]
		except Exception:
			pass
		if last == "":
			last = "0"
		n = int(last)
		if number == -1:
			data["VALUE"] = to_str(n + 1)
			with open(fname, 'a') as outfile:
				outfile.write(f"{data['VALUE']};{data['name']};{data['age']}\n")
		else:
			data["VALUE"] = to_str(number + 1)
			with open(fname, "w") as outfile:
				outfile.write(out)
	except Exception as e:
		pass

def parse_form_data(name):
	data = {}
	data["name"] = ""
	data["age"] = ""
	data["VALUE"] = ""
	data["NAME"] = ""
	form_data = ""
	
	method = os.getenv("REQUEST_METHOD", "")
	if method == "POST":
		form_data = sys.stdin.read()
	elif method == "GET":
		form_data = os.getenv("QUERY_STRING", "")
		if check_cookie(data, name):
			return data
	else:
		return data

	
	for token in form_data.split('&'):
		if '=' in token:
			key, value = token.split('=', 1)
			key = url_decode(key)
			value = url_decode(value)
			data[key] = value
	
	if not (method == "GET" and os.getenv("QUERY_STRING", "") == ""):
		add_to_base(data, name)
	
	return data

def done(data):
	
	print(f"Set-Cookie: {data['NAME']}={data['VALUE']}\r\n", end="")
	print("Content-Type: text/html\r\n\r\n", end="")
	print("<!DOCTYPE html>\r\n", end="")
	print('<html lang="en">\r\n', end="")
	print("<head><meta charset=\"UTF-8\"><title>Form Response</title></head>\r\n", end="")
	print("<body>\r\n", end="")
	print("<h1>Form Data Received</h1>\r\n", end="")
	print(f"<p>Name: {data['name']}</p>\r\n", end="")
	print(f"<p>Age: {data['age']}</p>\r\n", end="")
	print("</body>\r\n", end="")
	print("</html>\r\n", end="")
	sys.exit(0)

if __name__ == "__main__":
	name = "obritt_cgi_py"
	data = parse_form_data(name)
	done(data)
