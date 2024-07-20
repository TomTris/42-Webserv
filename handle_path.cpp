
#include "webserv.hpp"

bool isDirectory(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return false; // Error accessing the path
    }
    return S_ISDIR(path_stat.st_mode);
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
    return locations[ind];
}

std::string get_path_to_file(Server& s, std::string& path)
{
    std::string similar = "";
    int ind = get_most_fitting(s.locations, path, similar);
    std::string rooted = s.locations[ind].root;
    // std::cout << std::endl << similar << "<-------" << std::endl << std::endl <<  std::endl;
    // std::string output = path.substr(url.size(), path.size());

    
    
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
        std::vector<std::string> indexes = s.locations[ind].indexes;
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
                found = 1;
                break;
            }
        }
        if (!found)
        {
            std::cout << "Error directory and not index found" << std::endl;
        }
        
    }
    if (output[0] == '/')
    {
        output = output.substr(1);
    }
    return output;
}