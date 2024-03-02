#include <cm_node.h>

void node::node(const std::string& in_name, const std::string& in_label)
{
    name = in_name;
    label = in_label;
}

void node::add_feature(const string& feature) 
{
    features.push_back(feature);
}

string node::get_name()
{
    return(name);
}

string node::get_label()
{
    return(label);
}

vector<string> node::get_features()
{
    return(features):
}