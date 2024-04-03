/* external sources */
#include <iostream>
#include <sstream>
#include <experimental/filesystem>
#include <string>
#include <vector>

/* codemapper sources */
#include <cm_graph.h>
#include <cm_lan.h>
#include <tools.h>

namespace fs = std::experimental::filesystem;

language::language(const string& in_target)
{    
    target = in_target;
}

cm_graph* language::parse()
{    
    std::string foldername = lastname(target,CM_SYS_SPLITER_CHAR);
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
            std::cerr << "looking for sources: " << input;
            size_t pos = input.path().string().find("build");
            if (pos!=string::npos)
            {
                std::cerr << DEBUG_REDTXT << " ignored because build keyword found" << DEBUG_RESTXT << std::endl;                                
                continue;
            }
            pos = input.path().string().find(".git");
            if (pos!=string::npos)
            {
                std::cerr << DEBUG_REDTXT << " ignored because git keyword found" << DEBUG_RESTXT << std::endl;                
                continue;
            }
            pos = input.path().string().find(CM_SYS_SPLITER_CHAR+'.');
            if (pos!=string::npos)
            {
                std::cerr << DEBUG_REDTXT << " ignored becaouse dot special char found" << DEBUG_RESTXT << std::endl;                
                continue;                                
            }
            std::cerr << std::endl;

            std::cerr << "\t" << input.path() << " =======> ";
            std::string name = lastname(input.path().string(),CM_SYS_SPLITER_CHAR);
            std::cerr << name << std::endl;
        
            if (name != "")
            {
                std::string file_extension = lastname(input.path().string(),'.');
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
                            string new_name = lastwo(sourcesmap[name],CM_SYS_SPLITER_CHAR,CM_GLOBAL_JOIN_CHAR);
                            std::cerr << "\t\t\t" << name << DEBUG_MGTTXT << " erased and changed by -> " << new_name << DEBUG_RESTXT << " : " << old_path << std::endl;
                            sourcesmap.erase(name);
                            erasestring(sourceslist,name);
                            sourceslist.push_back(new_name);                            
                            sourcesmap[new_name] = old_path;
                            /* change current insertion name */
                            new_name = lastwo(input.path().string(),CM_SYS_SPLITER_CHAR,CM_GLOBAL_JOIN_CHAR);
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

language::~language()
{

}
