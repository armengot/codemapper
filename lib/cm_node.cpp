#include <cm_node.h>

cm_node::cm_node(const std::string& in_name, const std::string& in_label)
{
    name = in_name;
    label = in_label;
}

void cm_node::add_feature(const string& feature) 
{
    features.push_back(feature);
}

string cm_node::get_name()
{
    return(name);
}

string cm_node::get_label()
{
    return(label);
}

vector<string> cm_node::get_features()
{
    return(features);
}