# Use the official Debian base image
FROM debian:latest

# Install Apache and Python
RUN apt-get update && \
    apt-get install -y apache2 python3 && \
    apt-get clean

# Enable CGI module
RUN a2enmod cgi

# Set up the CGI directory
RUN mkdir -p /usr/lib/cgi-bin && \
    chmod +x /usr/lib/cgi-bin

# Copy your CGI script to the container
COPY calculator.py /usr/lib/cgi-bin/calculator.py

# Set permissions for your CGI script
RUN chmod +x /usr/lib/cgi-bin/calculator.py && chmod 755 /usr/lib/cgi-bin


# Configure Apache to use CGI
COPY apache-config.conf /etc/apache2/sites-available/000-default.conf

# Expose port 80
EXPOSE 80

# Start Apache
CMD ["apachectl", "-D", "FOREGROUND"]
