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
            std::cerr << "looking for sources: " << input << std::endl;
            size_t pos = input.path().string().find("build");
            if (pos!=string::npos)
                continue;
            pos = input.path().string().find(".git");
            if (pos!=string::npos)
                continue;
            pos = input.path().string().find("/.");
            if (pos!=string::npos)
                continue;                                

            //std::cerr << "\t" << input.path() << std::endl;
            std::string name = lastname(input.path(),'/');
        
            if (name != "")
            {
                std::string file_extension = lastname(input.path(),'.');
                if (file_extension != "")
                {
                    if ((file_extension==src_ext)||(file_extension==lib_ext))
                    {
                        auto it = sourcesmap.find(name);
                        /* if a name equal in list */
                        if (it!=sourcesmap.end())
                        {
                            std::cerr << "\t\t\tfound previous " << name << " : " << sourcesmap[name] << std::endl;
                            /* change name of old */
                            string old_path = sourcesmap[name];
                            string new_name = lastwo(sourcesmap[name],'/',global_join_char);
                            std::cerr << "\t\t\t" << name << " erased and changed by -> " << new_name << " : " << old_path << std::endl;
                            sourcesmap.erase(name);
                            erasestring(sourceslist,name);
                            sourceslist.push_back(new_name);                            
                            sourcesmap[new_name] = old_path;
                            /* change current insertion name */
                            new_name = lastwo(input.path(),'/',global_join_char);
                            sourceslist.push_back(new_name);
                            sourcesmap[new_name] = input.path().string();
                            std::cerr << "\t" << new_name << " : " << DEBUG_BLDTXT << DEBUG_GRNTXT << sourcesmap[new_name] << DEBUG_RESTXT << std::endl;
                        }
                        else
                        {
                            sourceslist.push_back(name);
                            sourcesmap[name] = input.path().string();
                            std::cerr << "\t" << name << " : " << DEBUG_BLDTXT << DEBUG_GRNTXT << sourcesmap[name] << DEBUG_RESTXT << std::endl;                            
                        }
                    }
                }
            }
        }
    }
    std::cerr << "\nWORKING MAP\n";
    for (const auto& pair : sourcesmap) 
    {
        std::cerr << pair.first << ":\t" << pair.second << std::endl;
    }
}

void language::lookfor_deps()
{

}

language::~language()
{

}
