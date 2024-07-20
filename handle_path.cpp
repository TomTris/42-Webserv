
#include "webserv.hpp"

bool isDirectory(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return false; // Error accessing the path
    }
    return S_ISDIR(path_stat.st_mode);
}

std::string get_path_to_file(location& loc, std::string path)
{
    std::string similar = loc.url;
    std::string rooted = loc.root;
    std::string temp =  path.substr(similar.size());
    if (temp[0] != '/')
    {
        temp = "/" + temp;
    }
    if (rooted[0] == '/')
    {
        rooted = rooted.substr(1);
    }
    std::string output = rooted + temp;
    if (isDirectory(output.c_str()))
    {
        std::vector<std::string> indexes = loc.indexes;
        bool found = 0;
        for (int i = 0; i < indexes.size(); i++)
        {
            temp = indexes[i];
            if (temp[0] != '/')
            {
                temp = "/" + temp;
            }
            output = rooted + temp;
            if (access(output.c_str(), F_OK) == 0)
            {
                break;
            }
        }

    }
    if (output[0] == '/')
    {
        output = output.substr(1);
    }
    return output;
}