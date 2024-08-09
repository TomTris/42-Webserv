#!/bin/sh

# query_string="$QUERY_STRING"

# redirect_high="https://www.youtube.com/watch?v=O_17i-ZoIzg"
# redirect_low="https://www.youtube.com/watch?v=IeYJbDboFdw"


# height=$(echo "$query_string" | awk -F'height=' '{print $2}' | awk -F'&' '{print $1}')

# # Check if height is numeric and greater than 180
# ret="Status: 301"
# ret+=\r\n
# ret+="Location: "

# if [ -n "$height" ] && [ "$height" -gt 180 ]; then
#   ret+=$redirect_high
# else
#   ret+=$redirect_low
# fi

# ret+=\r\n\r\n

# echo $ret

# # End of HTTP headers
# Get the query string from the environment variable
query_string="$QUERY_STRING"

# Define the redirect URLs
redirect_high="https://www.youtube.com/watch?v=O_17i-ZoIzg"
redirect_low="https://www.youtube.com/watch?v=IeYJbDboFdw"

# Extract the height parameter from the query string
height=$(echo "$query_string" | awk -F'height=' '{print $2}' | awk -F'&' '{print $1}')

# Determine the redirect URL based on the height value
if [ -n "$height" ] && [ "$height" -gt 180 ]; then
  redirect_url=$redirect_high
else
  redirect_url=$redirect_low
fi

# Output the HTTP headers
printf "Status: 301 Moved Permanently\r\n"
printf "Location: %s\r\n" "$redirect_url"
printf "Content-Length: 0\r\n";
printf "\r\n"  # End of HTTP headers

exit 0