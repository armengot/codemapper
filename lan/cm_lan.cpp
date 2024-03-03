#include <sstream>
#include <iostream>

#include <cm_lan.h>
#include <cm_graph.h>

language::language(const string& in_target)
{
    target = in_target;
}

cm_graph language::parse()
{
    std::vector<std::string> parts;
    std::istringstream ss(target);
    std::string parte;
    while (std::getline(ss, parte, '/')) 
    {
        parts.push_back(parte);
    }    
    std::string foldername = parts.back();

    cm_graph task(foldername);

    std::cout << task.to_string() << std::endl;;

    return task;
}

language::~language()
{

}
