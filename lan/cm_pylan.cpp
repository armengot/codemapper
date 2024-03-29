/* external sources */
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <vector>

/* codemapper sources */
#include <cm_graph.h>
#include <cm_lan.h>
#include <cm_pylan.h>
#include <tools.h>

namespace fs = std::experimental::filesystem;

py_language::py_language(const string& in_target) : language(in_target)
{    
    src_ext = "py";
    lib_ext = "py";
    inc_key = "import";   
}

void py_language::create_nodes(cm_graph* project)
{    
    std::vector<std::string> sources;
    
    for (const auto& source : sourceslist)
    {
        std::string ext = lastname(source,'.');
        if (ext == src_ext)        
        {
            sources.push_back(source);
            std::cerr << "source: " << source << std::endl;
        }
    }
    for (auto source = sources.begin(); source != sources.end(); ++source)
    {
        string namesource = firstname(*source,'.');
        //std::cerr << "\tprocessing: " << nameheader << std::endl;
        if (namesource!="")
        {
            for (auto candidate = sources.begin(); candidate != sources.end(); ++candidate)
            {
                string clean,candidate_name = firstname(*candidate,'.');

                //std::cerr << "\t\tprocessing: " << nameheader << " : " << namesource << std::endl;
                if (namesource == candidate_name)
                {
                    if ((namesource.find('.')!=string::npos))                    
                    {                        
                        clean = firstname(namesource,'.');
                    }
                    else
                    {
                        clean = namesource;
                    }
                    cm_node* source_node = new cm_node(clean,candidate_name);
                    source_node->add_feature("shape = box");
                    //headers.erase(header);
                    //sources.erase(candidate);
                    project->addnode(source_node);
                }                
            }            
        }
    }
    if (!sources.empty())
    {
        for (const auto& source : sources)
        {
            cm_node* alone;
            if (charin(CM_GLOBAL_JOIN_CHAR,source))
            {
                string label = lastwo(source,CM_GLOBAL_JOIN_CHAR,CM_SYS_SPLITER_CHAR);
                alone = new cm_node(firstname(source,'.'),label);
            }
            else
            {
                alone = new cm_node(firstname(source,'.'),source);
            }
            alone->add_feature("shape = box");
            project->addnode(alone);
        }
    }
}

void py_language::create_edges(cm_graph* project)
{
    for (const auto& pair : sourcesmap) 
    {
        std::cerr << "Looking for deps: " << pair.first << "  absolute path: " << pair.second << std::endl;
        cm_node* tail = project->lookfor(pair.first);
        std::cerr << "\tEquivalent node: " << tail->get_name() << std::endl;
        std::ifstream file(pair.second);
        if (file.is_open()) 
        {
            std::string line;
            int line_number = 0;
            while (std::getline(file, line)) 
            {
                line_number++;            
                if (line.find(inc_key) != std::string::npos)
                {
                    std::cerr << "\tFound " << DEBUG_BLDTXT << inc_key << DEBUG_RESTXT << " in "  << pair.second << " at line " << line_number << ": " << line << std::endl;
                    for (auto& node : project->allnodes()) 
                    {
                        string name_node = node->get_name();
                        //std::cerr << "\t\tlooking for " << name_node << "   in   " << line << std::endl;
                        if (line.find(name_node) != std::string::npos)
                        {                            
                            cm_node* head = project->lookfor(name_node);                            
                            cm_edge* new_edge = new cm_edge("","",tail,head);
                            if (project->addedge(new_edge))
                                std::cerr << DEBUG_GRNTXT << "\t\t" << tail->get_name() << " -> " << head->get_name() << DEBUG_RESTXT << std::endl;

                        }
                    }
                }
            }
            file.close();
        }
        else 
        {
            std::cerr << "Unable to open file: " << pair.second << std::endl;
        }        

    }
}




