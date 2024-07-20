

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>


bool isDirectory(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return false; // Error accessing the path
    }
    return S_ISDIR(path_stat.st_mode);
}

int main()
{
    std::string a = "/";
    std::string b = "/";

    std::cout << isDirectory("www/") << std::endl;
}