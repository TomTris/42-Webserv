
#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>


int my_atoi(std::string numb);
int get_ip_as_number(std::string add);
void set_bytes_to_zero(void *start, int len);
void createIPv4address(sockaddr_in* address, std::string ip, int port);
int createSocket();

#endif