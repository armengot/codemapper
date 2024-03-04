/* external headers */
#include <iostream>
#include <string>
#include <sstream>
/* codemapper headers */
#include <cm_graph.h>
#include <cm_node.h>
#include <cm_edge.h>

cm_graph::cm_graph(const std::string& in_name)
{
    name = in_name;
}

cm_graph::~cm_graph()
{   
    /*
    for (auto cm_node : nodes) 
    {
        delete cm_node;
    }
    for (auto cm_edge : edges) 
    {
        delete cm_edge;
    }
    */
}

void cm_graph::edgesall(const string& feature)
{
    for (const auto& edge : edges) 
    {
        edge->add_feature(feature);
    }      
}

void cm_graph::addnode(cm_node* node)
{
    nodes.push_back(node);
}

void cm_graph::addedge(cm_edge* edge)
{
    edges.push_back(edge);
}

cm_node* cm_graph::lookfor(std::string name)
{
    for (const auto& node : nodes) 
    {
        if (name == node->get_name())
        {
            return(node);
        }
    }
    return(nullptr);
}

std::string cm_graph::to_string() const 
{
    std::stringstream ss;
    ss << "digraph " << name << "\n{\n";

    // Generar nodos
    for (const auto& node : nodes) 
    {
        ss << "\t" << node->get_name() << "\n\t[\n";
        ss << "\t\tlabel=\"" << node->get_label() << "\"\n";
        
        std::vector<std::string> features = node->get_features();
        for (const auto& feature : features) 
        {
            ss << "\t\t" << feature << "\n";
        }
        
        ss << "\t];\n";
    }
    
    // Generar arcos
    for (const auto& edge : edges) 
    {
        ss << "\t" << edge->get_tail()->get_name() << " -> " 
           << edge->get_head()->get_name() << "\n\t[\n";
        ss << "\t\tlabel=\"" << edge->get_label() << "\"\n";
        
        std::vector<std::string> features = edge->get_features();
        for (const auto& feature : features) 
        {
            ss << "\t\t" << feature << "\n";
        }
        
        ss << "\t];\n";
    }

    ss << "}\n";
    return ss.str();
}
