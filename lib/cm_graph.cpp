#include <cm_graph.h>
#include <cm_node.h>
#include <cm_edge.h>

void cm_graph::cm_graph(const std::string& in_name)
{
    name = in_name;
}

void cm_graph::~graph(); 
{        
    for (auto node : nodes) 
    {
        delete node;
    }
    for (auto edge : edges) 
    {
        delete edge;
    }
}    

void addnode(node* node)
{
    nodes.push_back(node);
}
void addedge(edge* edge)
{
    edges.push_back(edge);
}

std::string cm_graph::to_string() const 
{
    std::stringstream ss;
    ss << "digraph {\n";

    for (const auto& node : nodes) 
    {
        ss << "    " << node->get_name() << " [label=\"\n" << node->get_label();
        std::vector<std::string> features = node->get_features();
        if (!features.empty())
        {
            ss << "\\n";
            for (const auto& feature : features) 
            {
                ss << "\\n" << feature;
            }
        }
        ss << "\"];\n";
    }
    
    for (const auto& edge : edges) 
    {
        ss << "    " << edge->get_tail()->get_name() << " -> " << edge->get_head()->get_name() << " [label=\"" << edge->get_label();
        std::vector<std::string> features = edge->get_features();
        if (!features.empty())
        {
            ss << "\\n";
            for (const auto& feature : features) 
            {
                ss << "\\n" << feature;
            }
        }
        ss << "\"];\n";
    }


    ss << "}\n";
    return ss.str();
}