/* external sources */
#include <iostream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>

/* codemapper sources */
#include <cm_graph.h>
#include <cm_lan.h>
#include <tools.h>

namespace fs = std::filesystem;

language::language(const string& in_target)
{    
    target = in_target;
}

cm_graph* language::parse()
{    
    std::string foldername = lastname(target,'/');
    cm_graph* task=new cm_graph(foldername);    
    lookfor_sources();
    return task;
}

void language::lookfor_sources()
{
    for (const auto& input : fs::recursive_directory_iterator(target)) 
    {
        if (fs::is_regular_file(input))
        {
            size_t pos = input.path().string().find("build");
            if (pos!=string::npos)
                continue;
            pos = input.path().string().find(".git");
            if (pos!=string::npos)
                continue;
            pos = input.path().string().find("/.");
            if (pos!=string::npos)
                continue;                                

            std::cout << input.path();
            std::string name = lastname(input.path(),'/');
        
            if (name != "")
            {
                std::string file_extension = lastname(input.path(),'.');
                if (file_extension != "")
                {
                    if ((file_extension==src_ext)||(file_extension==lib_ext))
                    {
                        sourceslist.push_back(name);
                        std::cout << " " << name << std::endl;
                    }
                }
            }
        }
    }
}

void language::lookfor_deps()
{

}

language::~language()
{

}
