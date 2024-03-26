#include <cm_edge.h>

cm_edge::cm_edge(const string& in_name, const string& in_label, cm_node* in_tail, cm_node* in_head)
{
    name = in_name;    
    label = in_label;
    tail = in_tail;
    head = in_head;
}

void cm_edge::add_feature(const string& feature) 
{
    features.push_back(feature);
    
}

cm_node* cm_edge::get_head()
{
    return(head);
}
cm_node* cm_edge::get_tail()
{
    return(tail);
}

std::string cm_edge::get_name()
{
    return(name);
}

std::string cm_edge::get_label()
{
    return(label);
}

std::vector<std::string> cm_edge::get_features()
{
    return(features);
}
string cm_edge::humanreadable()
{    
    std::string edge_humanreadable = tail->get_name()+" --> "+head->get_name();
    return(edge_humanreadable);
}

                    