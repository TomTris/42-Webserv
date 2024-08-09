#include "../Tomweb.hpp"

int count_amount_of_app(std::string path, char c)
{
    int counter = 0;
    for (unsigned int i = 0; i < path.size(); i++)
    {
        if (path[i] == c)
        {
            counter++;
        }
    }
    return (counter);
}

bool checkIfFileExistsAndNotDirectory(std::string& path)
{
    struct stat path_stat;

    if (stat(path.c_str(), &path_stat) == 0) {
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
    int max = 0;
    int ind = -1;
    for (unsigned int i = 0; i < locations.size(); i++)
    {
        if (locations[i].URI.size() > path.size())
            continue;

        if (path.find(locations[i].URI) == 0)
        {
            if (path.size() == locations[i].URI.size())
                return (locations[i]);
            if (count_amount_of_app(locations[i].URI, '/') > max)
            {
                ind = i;
                max = count_amount_of_app(locations[i].URI, '/');
            }
        }
    }
    if (ind == -1)
        throw std::runtime_error("asdas");
    return locations[ind];
}

std::vector<std::string> get_allowed(location& loc)
{
    if (loc.allowed.size() == 0)
    {
        std::vector<std::string> vec(3);
        vec[0] = "POST";
        vec[1] = "DELETE";
        vec[2] = "GET";
        return vec;
    }
    return loc.allowed;
}

bool isAllowed(location& loc, std::string& method)
{
    std::vector<std::string> allowed = get_allowed(loc);
    if (find(allowed.begin(), allowed.end(), method) != allowed.end())
        return true;
    return false;
}

//should be valid errorNumb if defind new please add it to function

std::string get_path_of_standart_error(int errorNumb)
{
    std::map<int, std::string> mp;

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

bool isDirectory(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return false; // Error accessing the path
    }
    return S_ISDIR(path_stat.st_mode);
}

std::string get_path_to_file(location& loc, std::string path)
{
    int seen = 0;
    std::string similar = loc.URI;
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
    std::string saved = output;
    if (isDirectory(output.c_str()))
    {
        std::vector<std::string> indexes = loc.indexes;
        // bool found = 0;
        for (unsigned int i = 0; i < indexes.size(); i++)
        {
            temp = indexes[i];
            if (temp[0] != '/')
            {
                temp = "/" + temp;
            }
            output = rooted + temp;
            while (output[0] == '/' and output.size() != 0)
                output = output.substr(1);
            if (access(output.c_str(), F_OK) == 0)
            {
                seen = 1;
                break;
            }
        }

    }
    if (output[0] == '/')
    {
        output = output.substr(1);
    }
    if (seen)
        return output;
    return saved;
}

int getIndexLocation(std::string &host, Server& serv)
{
    for (unsigned int i = 0; i < serv.server_names.size(); i++)
    {
        if (serv.server_names[i] == host)
            return (i);
    }
    if (serv.server_names[0] == "")
        return (0);
    return (-1);
}

bool isHostOk(std::string &host, Server& serv)
{
    if (host == "")
        return 1;
    if (host.find(':') == std::string::npos)
    {
        return 0;
    }
    std::string h = host.substr(0, host.find(':'));
    host = host.substr(host.find(':') + 1);
    if (host.size() > 5 || !isNumber(host))
        return 0;
    if (getIndexLocation(host, serv) == -1)
        return (0);
    int port = my_atoi(host);
    if (serv.port != port || port > 0XFFFF)
        return (0);
    host = h;
    return 1;
}

std::string get_ex(std::string url)
{
    int i = (int)url.size() - 2;
    for (; i >= -1; i--)
    {
        if (i == -1 || url[i] == '\\')
            return "";
        if (url[i] == '.')
            break;
    }
    std::string a = url.substr(i);
    a.pop_back();
    return (a);
}

// # 1. host (like: localhost:8081 -> localhost is servername) std::string
// # 2. Method std::string
// # 3. URL std::string
// # 4. Port not Port but Server
// # then you answer me 1. Is Host:Port ok ( function return "0"|"1") ? 2.  method allow or not 3. what the URL is.
// 4. Auto index "0"|"1"
// 5.

std::vector<std::string> get_data(std::string host, std::string method, std::string url, Server& serv)
{
    std::vector<std::string> output;
    handle_URI(url);
    if (url.back() != '/')
        url.push_back('/');
    std::string ex = get_ex(url);
    if (!isHostOk(host, serv))
    {
        output.push_back("0");
        output.push_back("0");
        output.push_back("0");
        output.push_back("0");
        output.push_back("");
        output.push_back("");
        output.push_back("");
        return (output);
    }
    output.push_back("1");
    std::vector<location> locations = serv.locations[getIndexLocation(host, serv)];
    location loc;
    try
    {
        loc = get_location(locations, url);
    }
    catch (const std::runtime_error&e) 
    {
        output.push_back("1");
        output.push_back(url);
        output.push_back(loc.autoindex ? "1" : "0");
        output.push_back("");
        output.push_back("");
        output.push_back("");
        return (output);
    }
    if (isAllowed(loc, method))
    {
        output.push_back("1");
    }
    else
    {
        output.push_back("0");
        output.push_back("0");
        output.push_back("0");
        output.push_back("");
        output.push_back("");
        output.push_back("");
        return (output);
    }
    std::string path = get_path_to_file(loc, url);
    while (path.back() == '/' && path.size() != 1)
        path.pop_back();
    output.push_back(path);
    if (loc.autoindex)
    {
        output.push_back("1");
    }
    else
    {
        output.push_back("0");
    }
    output.push_back(loc.returning);
    int seen = 0;
    for (unsigned int i = 0; i < loc.cgi_ex.size(); i++)
    {
        if (loc.cgi_ex[i] == ex)
        {
            output.push_back(loc.cgi_path[i]);
            output.push_back(loc.cgi_ex[i]);
            seen = 1;
            break;
        }
    }
    if (!seen)
    {
        output.push_back("");
        output.push_back("");
    }
    return (output);
}
