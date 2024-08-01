
#include "../Tomweb.hpp"

void set_bytes_to_zero(void *start, int len)
{
    char *transformed = reinterpret_cast<char *>(start);

    for (int i = 0; i < len; i++)
        transformed[i] = 0;
}

long long my_atoi(std::string numb)
{
    std::stringstream ss(numb);
    long long num;

    ss >> num;
    return num;
}

int get_ip_as_number(std::string add, int& err)
{
    int out = 0;
    int numb = 0;
    for (int i = 0; i < 3; i++)
    {
        if (add.find('.') == 0)
        {
            throw std::runtime_error("error wrong value host");
            err = 1;
            return 0;
        }
        std::string temp = add.substr(0, add.find("."));
        if (temp == "")
        {
            err = 1;
            return 0;
        }
        add = add.substr(add.find(".") + 1, add.size());
        out <<= 8;
        numb = my_atoi(temp);
        if (numb > 255)
        {
            throw std::runtime_error("error wrong value host");
            err = 1;
            return 0;   
        }
        out |= numb;
    }
    out <<= 8;
    numb = my_atoi(add);
    if (add == "")
    {
        throw std::runtime_error("error wrong value host");
        err =1;
        return 0;
    }
    if (numb > 255)
    {
        throw std::runtime_error("error wrong value host");
        err = 1;
        return 0;
    }
    out |= numb;
    return out;
}

void handle_URI(std::string& URI)
{
    int ptr = 0;
    while (URI.substr(ptr).find('/') != std::string::npos)
    {
        ptr += URI.substr(ptr).find('/');
        ptr += 1;
        int len = 0;
        while (URI.size() > ptr + len && URI[ptr + len] == '/')
            len++;
        URI.erase(ptr, len);
    }
	if (URI[0] != '/')
		URI = "/" + URI;
}

bool isWhiteSpace(char c)
{
    if (c <= 9 && c >= 13)
        return true;
    if (c == ' ')
        return true;
    return false;
}

bool isNumber(std::string& number)
{
    for (int i = 0; i < number.size(); i++)
    {
        if (!(number[i] >= '0' && number[i] <= '9'))
            return false;
    }
    return true;
}

std::string getFistWordAndDelete(std::string& str)
{
    std::string out = "";
    int i = 0;
    for (; i < str.size(); i++)
    {
        if (str[i] == ' ')
            break ;
        out.push_back(str[i]);
    }
    if (i != str.size())
        str = str.substr(i + 1, str.size());
    else
        str = "";
    return out;
}

std::string update_spaces(std::string& input)
{
    std::string output = "";
    int flag = 1;
    for (int i = 0; i < input.size(); i++)
    {
        if (isWhiteSpace(input[i]))
        {
            if (flag)
                continue;
            flag = 1;
            output.push_back(input[i]);
            
            continue;
        }
        if (input[i] == '#')
        {
            if (output.size() >= 1 && isWhiteSpace(output[output.size() - 1]))
                output.pop_back();
            return (output);
        }
        if ((input[i] == '{' || input[i] == '}' || input[i] == ';') && output.back() == ' ')
            output.pop_back();
        output.push_back(input[i]);
        if (input[i] != '{' && input[i] != '}')
            flag = 0;
    }
    if (output.size() >= 1 && isWhiteSpace(output[output.size() - 1]))
        output.pop_back();
    return output;
}

bool areParanthesesOk(std::string& output)
{
    int count = 0;
    int seen = 0;
    for (int i = 0; i < output.size(); i++)
    {
        if (output[i] == '{')
        {
            count++;
            if (count > 2)
                return false;
        }
        if (output[i] == '}')
        {
            count--;
            if (count < 0)
                return false;
        }
    }
    if (count != 0)
        return false;
    return true;
}

std::vector<std::string> getserver_ts(std::string& output)
{
    std::vector<std::string> returnValue;
    int count = 0;
    int current = 0;
    int prev = 0;
    for (int i = 0; i < output.size(); i++)
    {
        if (output[i] == '{')
        {
            count++;
        }
        if (output[i] == '}')
        {
            count--;
            if (count == 0)
            {
                returnValue.push_back(output.substr(prev, current - prev + 1));
                prev = current + 1;
            }
        }
        current++;
    }
    return returnValue;
}

bool isLocation(std::string& serv)
{
    for (int i = 0; i < serv.size(); i++)
    {
        if (serv[i] == '{')
            return true;
        else if (serv[i] == ';')
            return false;
    }
    return true;
}

void handleHost(std::string &ip, server_t& s, int& err)
{
    ip.pop_back();
    if (!(ip.size() >= 7 && ip.size() <= 15))
    {
        throw std::runtime_error("error wrong value host");
        err =1;
        return ;
    }
    int count = 0;
    for (int i = 0; i < ip.size(); i++)
    {
        if (!(ip[i] == '.' || (ip[i] >= '0' && ip[i] <= '9')))
        {
            throw std::runtime_error("error wrong value host");
            err = 1;
            return ;
        }
        if (ip[i] == '.')
            count++;
    }
    if (count != 3)
    {
        throw std::runtime_error("error wrong value host");
        err = 1;
        return ;
    }
    s.host = get_ip_as_number(ip, err);
}

void handlePort(std::string &port, server_t& s, int& err)
{
    port.pop_back();
    if (port.size() == 0 || port.size() > 5 || !isNumber(port))
    {
        throw std::runtime_error("error wrong value port");
        err = 1;
        return ;
    }
    int temp = my_atoi(port);
    if (temp > 0xFFFF)
    {
        throw std::runtime_error("error wrong value port");
        err = 1;
        return ;
    }
    s.port = temp;
}

void handleserverName(std::string& servername, server_t& s, int& err)
{
    servername.pop_back();
    if (servername == "_")
        return ;
    if (servername.find(' ') != std::string::npos)
    {
        throw std::runtime_error("error server name wrong syntaxes");
        err = 1;
        return ;
    }
    s.servername = servername; 
}

void handleErrorPage(std::string& errorPage, server_t& s, int& err)
{
    errorPage.pop_back();
    if (errorPage.find(' ') == std::string::npos)
    {
        throw std::runtime_error("error page wrong syntaxes");
        err = 1;
        return ;
    }
    std::string errCode = getFistWordAndDelete(errorPage);
    if (errCode.size() > 9 || !isNumber(errCode))
    {
        throw std::runtime_error("error page wrong value");
        err = 1;
        return;
    }
    long long error = my_atoi(errCode);
    if (error > 2147483647)
    {
        throw std::runtime_error("error page wrong value");
        err = 1;
        return ;
    }
    s.errorpages[error] = errorPage;
}

void handleMaxBody(std::string& body, server_t& s, int& err)
{
    body.pop_back();
    if (body.find(' ') != std::string::npos)
    {
        throw std::runtime_error("body_limits wrong syntaxes");
        err = 1;
        return ;
    }
    if (body.size() > 9 || !isNumber(body))
    {
        throw std::runtime_error("body_limits wrong value");
        err = 1;
        return;
    }
    long long numb = my_atoi(body);
    if (numb > 2147483647)
    {
        throw std::runtime_error("body_limits wrong value");
        err = 1;
        return ;
    }
    s.client_max_body_size = numb;
}

void add_to_loc(std::string& temp, location& loc, int &err)
{
    std::string possibles[4] = {"PUT", "GET", "DELETE", "POST"};

    int i = 0;
    for (; i < 4; i++)
    {
        if (possibles[i] == temp)
        {
            break;
        }
    }
    if (i == 4)
    {
        throw std::runtime_error("allowed method wrong value");
        err = 1;
        return ;
    }
    if (std::find(loc.allowed.begin(), loc.allowed.end(), possibles[i]) == loc.allowed.end())
    {
        loc.allowed.push_back(possibles[i]);
    }
}

void handleIndex(std::string& indexes, location& loc)
{
    while (indexes.find(' ') != std::string::npos)
    {
        loc.indexes.push_back(getFistWordAndDelete(indexes));
    }
    loc.indexes.push_back(indexes);
}

void handleRoot(std::string& root, location& loc, int& err)
{
    if (root.find(' ') != std::string::npos)
    {
        throw std::runtime_error("root wrong syntaxes");
        err = 1;
        return ;
    }
    loc.root = root;
    handle_URI(loc.root);
}

void handleAllowMethods(std::string& methods, location &loc, int& err)
{
    std::string temp;
    while (methods.find(' ') != std::string::npos)
    {
        temp = methods.substr(0, methods.find(' '));
        methods = methods.substr(methods.find(' ') + 1, methods.size());
        add_to_loc(temp, loc, err);
    }
    add_to_loc(methods, loc, err);
}

void handleAutoindex(std::string& str, location &loc, int &err)
{
    if (str.find(' ') != std::string::npos)
    {
        throw std::runtime_error("autoindex wrong syntaxes");
        err = 1;
        return ;
    }
    if (str == "on" || str == "off")
    {
        loc.autoindex = (str == "on");
    }
    else
    {
        throw std::runtime_error("autoindex wrong value");
        err = 1;
    }
}

void handleCgiPath(std::string& path, location& loc, int &err)
{
    std::string temp; 
    while (path.find(' ') != std::string::npos)
    {
        temp = path.substr(0, path.find(' '));
        path = path.substr(path.find(' ') + 1, path.size());
        if (temp[0] != '/' || access(temp.c_str(), F_OK) != 0)
        {
            throw std::runtime_error("cgi_path wrong syntaxes");
            err = 1;
            return ;
        }
        if (find(loc.cgi_path.begin(), loc.cgi_path.end(), temp) == loc.cgi_path.end())
            loc.cgi_path.push_back(temp);
    }
    temp = path;
    if (temp[0] != '/' || access(temp.c_str(), F_OK) != 0)
    {
        throw std::runtime_error("cgi_path wrong syntaxes");
        err = 1;
        return ;
    }
    if (find(loc.cgi_path.begin(), loc.cgi_path.end(), temp) == loc.cgi_path.end())
        loc.cgi_path.push_back(temp);
}

void handleCgiEx(std::string& exec, location& loc, int &err)
{
    std::string temp; 
    while (exec.find(' ') != std::string::npos)
    {
        temp = exec.substr(0, exec.find(' '));
        exec = exec.substr(exec.find(' ') + 1, exec.size());
        if (temp[0] != '.')
        {
            throw std::runtime_error("cgi_ex wrong syntaxes");
            err = 1;
            return ;
        }
        if (find(loc.cgi_ex.begin(), loc.cgi_ex.end(), temp) == loc.cgi_ex.end())
            loc.cgi_ex.push_back(temp);
    }
    temp = exec;
    if (temp[0] != '.')
    {
        throw std::runtime_error("cgi_ex wrong syntaxes");
        err = 1;
        return ; 
    }
    if (find(loc.cgi_ex.begin(), loc.cgi_ex.end(), temp) == loc.cgi_ex.end())
        loc.cgi_ex.push_back(temp);
}

void handleReturn(std::string& cURIoc, location& loc, int &err)
{
    if (cURIoc.find(' ') != std::string::npos)
    {
        throw std::runtime_error("return wrong syntaxes");
        err = 1;
        return ;
    }
    loc.returning = cURIoc;
}

void handleParamLocation(std::string& cURIoc, location& loc, int &err)
{
    if (cURIoc[0] == ' ')
    {
        cURIoc = cURIoc.substr(1, cURIoc.size());
    }
    if (cURIoc.find(';') == std::string::npos)
    {
        throw std::runtime_error("location wrong syntaxes");
        err  = 1;
        return ;
    }
    loc.autoindex = true;
    std::string param = cURIoc.substr(0, cURIoc.find(';'));
    cURIoc = cURIoc.substr(cURIoc.find(';') + 1, cURIoc.size());
    if (param.find(' ') == std::string::npos)
    {
        throw std::runtime_error("location wrong syntaxes");
        err = 1;
        return ;
    }
    std::string category = param.substr(0, param.find(' '));
    param = param.substr(param.find(' ') + 1);
    if (category == "allowed_methods")
    {
        handleAllowMethods(param, loc, err);
    }
    else if (category == "index")
    {
        handleIndex(param, loc);
    }
    else if (category == "root")
    {
        handleRoot(param, loc, err);
    }
    else if (category == "autoindex")
    {
        handleAutoindex(param, loc, err);
    }
    else if (category == "cgi_path")
    {
        handleCgiPath(param, loc, err);
    }
    else if (category == "cgi_ex")
    {
        handleCgiEx(param, loc, err);
    }
    else if (category == "return")
    {
        handleReturn(param, loc, err);
    }
    else
    {
        throw std::runtime_error("location wrong paramater");
        err = 1;
        return ;
    }
}

void handleLocation(std::string& Location, server_t& s, int& err)
{
    if (Location[0] == ' ')
    {
        Location = Location.substr(1, Location.size());
    }
    std::string currentLoc = Location.substr(0, Location.find('}') + 1);
    Location = Location.substr(Location.find('}') + 1, Location.size());
    if (currentLoc.find(' ') == std::string::npos)
    {
        throw std::runtime_error("location wrong syntaxes");
        err = 1;
        return;
    }
    std::string check = currentLoc.substr(0, currentLoc.find(' '));
    if (check != "location")
    {
        throw std::runtime_error("location wrong syntaxes");
        err = 1;
        return ;
    }
    currentLoc = currentLoc.substr(currentLoc.find(' ') + 1, currentLoc.size());
    check = currentLoc.substr(0, currentLoc.find('{'));
    if (check.size() == 0 || check[0] != '/')
    {
        throw std::runtime_error("location wrong syntaxes");
        err = 1;
        return ;
    }

    currentLoc = currentLoc.substr(currentLoc.find('{') + 1, currentLoc.size());
    int i = 0;
    location loc;
    loc.root = "";
    loc.URI = check;
    handle_URI(loc.URI);
    if (loc.URI.back() != '/')
        loc.URI.push_back('/');
    loc.returning = "";
    while (currentLoc != "}" && currentLoc != " }")
    {
        handleParamLocation(currentLoc, loc, err);
        if (err)
            return ;
        i++;
        
    }
    if ( loc.cgi_ex.size() != loc.cgi_path.size()) // toDo
    {
        throw std::runtime_error("location wrong syntaxes");
        err = 1;
        return ;
    }
    s.locations.push_back(loc);
}

void handleNotLocation(std::string& serv, server_t& s, int &err)
{
    if (serv[0] == ' ')
    {
        serv = serv.substr(1, serv.size());
    }
    std::string temp = serv.substr(0, serv.find(';') + 1);
    
    serv = serv.substr(serv.find(';') + 1, serv.size());

    std::string category = getFistWordAndDelete(temp);
    if (category == "host")
    {
        handleHost(temp, s, err);
    }
    else if (category == "port")
    {
        handlePort(temp, s, err);
    }
    else if (category == "server_name")
    {
        handleserverName(temp, s, err);
    }
    else if (category == "error_page")
    {
        handleErrorPage(temp, s, err);
    }
    else if (category == "client_max_body_size")
    {
        handleMaxBody(temp, s, err);
    }
    else
    {
        throw std::runtime_error("server wrong paramater");
        err = 1;
        return;
    }
}

server_t createserver_t(std::string& serv, int& err)
{
    server_t s;
    serv = serv.substr(7, serv.size());
    s.host = get_ip_as_number("127.0.0.1", err);
    s.port = 8080;
    s.servername = "";
    s.client_max_body_size = 1024;
    while (serv[0] != '}')
    {
        if (isLocation(serv))
        {
            handleLocation(serv, s, err);
        }
        else
        {
            handleNotLocation(serv, s, err);
        }
        
        if (err)
            return s;
    }
    return s;
}

int parse(std::string path, std::vector<server_t>& s)
{
    std::ifstream inputFile(path);
    if (!inputFile)
    {
        throw std::runtime_error(path);
        return (1);
    }
    std::string output = "";
    std::string line = "";
    int cURIy = 0;
    while (std::getline(inputFile, line))
    {
        line = update_spaces(line);
        output += line + " ";
    }
    output = update_spaces(output);
    if (!areParanthesesOk(output))
    {
       throw std::runtime_error("wrong syntaxes");
        return 1;
    }
    std::vector<std::string> server_ts = getserver_ts(output);
    // server_t *s1;
    int err = 0;
    for (int i = 0; i < server_ts.size(); i++)
    {
        if (server_ts[i].substr(0, 7) != "server{")
        {
            throw std::runtime_error("wrong syntaxes");
            return 1;
        }
        s.push_back(createserver_t(server_ts[i], err));
        // delete s1;
        if (err)
            return 1;
    }
    return 0;
}