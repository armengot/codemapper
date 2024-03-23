/* external headers */
#include <iostream>
#include <string>
#include <sstream>
/* codemapper headers */
#include <cm_graph.h>
#include <cm_node.h>
#include <cm_edge.h>
#include <tools.h>

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

int cm_graph::addnode(cm_node* node)
{
    for (auto& each_node : nodes)
    {        
        if (each_node->get_name()==node->get_name())
            return(0);
    }
    nodes.push_back(node);
    return(1);
}

int cm_graph::addedge(cm_edge* edge)
{
    cm_node* tail = edge->get_tail();
    cm_node* head = edge->get_head();

    if (tail==head)
        return(0);

    for (auto& each_edge : edges)
    {
        if ((tail == each_edge->get_tail())&&(head == each_edge->get_head()))
            return(0);
    }
    edge->add_feature("dir=back");
    edges.push_back(edge);
    return(1);
}

vector<cm_node*> cm_graph::allnodes()
{
    return(nodes);
}

cm_node* cm_graph::lookfor(std::string name)
{
    std::string copy;
    if (name.find('.') != std::string::npos)
    {
        copy = firstname(name,'.');
    }
    else
    {
        copy = name;
    }
    for (const auto& node : nodes) 
    {
        if (copy == node->get_name())
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

    // write nodes
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
    
    // write edges
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
