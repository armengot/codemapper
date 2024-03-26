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

void cm_edge::erasecolor()
{
    for (int i = features.size() - 1; i >= 0; --i)
    {
        const std::string& f = features[i];
        size_t pos = f.find("color");
        if (pos != std::string::npos)
        {
            features.erase(features.begin()+i);
        }
    }
}

void cm_edge::setcolor(std::string color)
{
    erasecolor();
    features.push_back("color = "+color);
}


string cm_edge::humanreadable()
{    
    std::string edge_humanreadable = tail->get_name()+" --> "+head->get_name();
    return(edge_humanreadable);
}

                    