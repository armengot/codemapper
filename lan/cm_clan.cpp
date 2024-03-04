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

void cpp_language::create_nodes(cm_graph* project)
{
    std::vector<std::string> headers;
    std::vector<std::string> sources;
    
    for (const auto& source : sourceslist)
    {
        std::string ext = lastname(source,'.');
        if (ext[0]=='h')
        {
            headers.push_back(source);
            std::cerr << "\tadding header: " << source << std::endl;
        }
        if (ext[0]=='c')
        {
            sources.push_back(source);
            std::cerr << "\tadding source: " << source << std::endl;
        }
    }
    for (auto header = headers.begin(); header != headers.end(); ++header)
    {
        string nameheader = firstname(*header,'.');
        //std::cerr << "\tprocessing: " << nameheader << std::endl;
        if (nameheader!="")
        {
            for (auto candidate = sources.begin(); candidate != sources.end(); ++candidate)
            {
                string clean,namesource = firstname(*candidate,'.');

                //std::cerr << "\t\tprocessing: " << nameheader << " : " << namesource << std::endl;
                if (nameheader == namesource)
                {
                    if ((namesource.find('.')!=string::npos))                    
                    {                        
                        clean = firstname(namesource,'.');
                    }
                    else
                    {
                        clean = namesource;
                    }
                    cm_node* couple = new cm_node(clean,"{"+*candidate+"|"+*header+"}");
                    couple->add_feature("shape = record");
                    //headers.erase(header);
                    //sources.erase(candidate);
                    project->addnode(couple);
                }                
            }            
        }
    }
    if (!sources.empty())
    {
        for (const auto& source : sources)
        {
            cm_node* alone = new cm_node(firstname(source,'.'),source);
            alone->add_feature("shape = box");
            project->addnode(alone);
        }
    }
    if (!headers.empty())
    {
        for (const auto& source : headers)
        {
            cm_node* alone = new cm_node(firstname(source,'.'),source);
            alone->add_feature("shape = box");
            project->addnode(alone);
        }
    }
}

void cpp_language::create_edges(cm_graph* project)
{
    for (const auto& pair : sourcesmap) 
    {
        std::cerr << "Source called: " << pair.first << "  absolute path: " << pair.second << std::endl;
    }
}




