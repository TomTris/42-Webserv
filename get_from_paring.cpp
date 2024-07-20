
#include "webserv.hpp"


bool checkIfFileExistsAndNotDirectory(std::string& path)
{
    struct stat path_stat;

    if (stat(path.c_str(), &path_stat) == 0) {
        // Check if it is a regular file
        if (S_ISREG(path_stat.st_mode)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

location get_location(std::vector<location>& locations, std::string& path)
{
    std::string closest_so_far = "";
    int ind = 0;
    for (int i = 0; i < locations.size(); i++)
    {
        if (locations[i].url.size() > path.size())
            continue;

        if (path.find(locations[i].url) == 0)
        {
            if (path.size() == locations[i].url.size())
            {
                return locations[i];
            }
            else if (locations[i].url == "/")
            {
                if (closest_so_far == "")
                {
                    closest_so_far = locations[i].url;
                    ind = i;
                }
            }
            else if (path[locations[i].url.size()] == '/')
            {
                if (locations[i].url.size() > closest_so_far.size())
                {
                    closest_so_far = locations[i].url;
                    ind = i;
                }
            }
        }
    }
    if (closest_so_far == "")
    {
        location loc;
        loc.doesntExist = 1;
        return loc;
    }
    return locations[ind];
}

std::vector<std::string> get_allowed(location& loc)
{
    if (loc.allowed.size() == 0)
    {
        std::vector<std::string> vec(4);
        vec[0] = "POST";
        vec[1] = "DELETE";
        vec[2] = "GET";
        vec[3] = "PUT";
        return vec;
    }
    return loc.allowed;
}

//should be valid errorNumb if defind new please add it to function
std::string get_path_of_standart_error(int errorNumb)
{
    std::map<int, std::string> mp;

    mp[404] = STANDARD404;
    mp[405] = STANDARD405;

    return mp[errorNumb];
}

std::string get_error_page(Server& serv, int errorNumb)
{
    std::map<int, std::string>::iterator it = serv.errorPages.find(errorNumb);
    if (it == serv.errorPages.end())
    {
        return get_path_of_standart_error(errorNumb);
    }
    else
    {
        std::string path = it->second;
        if (path[0] == '/')
            path = path.substr(1);
        if (checkIfFileExistsAndNotDirectory(path))
        {
            return path;
        } 
        else
        {
            return get_path_of_standart_error(errorNumb);
        }
    }
}