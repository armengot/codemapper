/* external sources */
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

/* codemapper sources */
#include <cm_graph.h>
#include <cm_lan.h>
#include <cm_clan.h>
#include <tools.h>

namespace fs = std::filesystem;

cpp_language::cpp_language(const string& in_target) : language(in_target)
{    
    src_ext = "cpp";
    lib_ext = "h";
    inc_key = "include";   
}

void cpp_language::createnodes(cm_graph* project)
{
    std::vector<std::string> headers;
    std::vector<std::string> sources;
    
    for (const auto& source : sourceslist)
    {
        std::string ext = lastname(source,'.');
        if (ext[0]=='h')
        {
            headers.push_back(source);
            std::cout << "\tadding header: " << source << std::endl;
        }
        if (ext[0]=='c')
        {
            sources.push_back(source);
            std::cout << "\tadding source: " << source << std::endl;
        }
    }
    for (auto header = headers.begin(); header != headers.end(); )
    {
        string nameheader = firstname(*header,'.');
        std::cout << "\tprocessing: " << nameheader << std::endl;
        if (nameheader!="")
        {
            for (auto candidate = sources.begin(); candidate != sources.end(); )
            {
                string namesource = firstname(*candidate,'.');
                std::cout << "\t\tprocessing: " << nameheader << " : " << namesource << std::endl;
                if (nameheader == namesource)
                {
                    std::cout << "equal" << std::endl;
                    cm_node* couple = new cm_node(namesource,"{"+*candidate+"|"+*header+"}");
                    couple->add_feature("shape = record");
                    //headers.erase(header);
                    //sources.erase(candidate);
                    project->addnode(couple);
                }
                break;
            }
            break;
        }        
        break;
    }
    if (!sources.empty())
    {
        for (const auto& source : sources)
        {
            cm_node* alone = new cm_node(source,source);
            alone->add_feature("shape = box");
            project->addnode(alone);
        }
    }
    if (!headers.empty())
    {
        for (const auto& source : headers)
        {
            cm_node* alone = new cm_node(source,source);
            alone->add_feature("shape = box");
            project->addnode(alone);
        }
    }
}






