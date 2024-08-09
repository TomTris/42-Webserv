#!/bin/sh

query_string="$QUERY_STRING"

redirect_high="https://www.youtube.com/watch?v=O_17i-ZoIzg"
redirect_low="https://www.youtube.com/watch?v=IeYJbDboFdw"

height=$(echo "$query_string" | awk -F'height=' '{print $2}' | awk -F'&' '{print $1}')

if [ -n "$height" ] && [ "$height" -gt 180 ]; then
  redirect_url=$redirect_high
else
  redirect_url=$redirect_low
fi

printf "Status: 301 Moved Permanently\r\n"
printf "Location: %s\r\n" "$redirect_url"
printf "Content-Length: 0\r\n";
printf "\r\n"

exit 0