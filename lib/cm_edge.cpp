#include <cm_edge.h>

void edge::edge(const string& in_name, const string& in_label, node* in_tail, node* in_head)
{
    name = in_name;
    label = in_label;
    tail = in_tail;
    head = in_head;
}

void edge::add_feature(const string& feature) 
{
    features.push_back(feature);
}

node* edge::get_head()
{
    return(head);
}
node* edge::get_tail()
{
    return(tail);
}

string edge::get_name()
{
    return(name);
}

string edge::get_label()
{
    return(label);
}

vector<string> edge::get_features()
{
    return(features):
}