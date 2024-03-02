#ifndef CM_EDGE_H
#define CM_EDGE_H

#include <string>
#include <vector>
#include <cm_node.h>

using namespace std;
/*
 ___ ___   ___ ___ 
| __|   \ / __| __|
| _|| |) | (_ | _| 
|___|___/ \___|___|

*/               
class edge 
{
    public:
        edge(const string& in_name, const string& in_label, node* tail, node* head);
        add_feature(string feature);
        node* get_head();
        node* get_tail();
        string get_name();
        string get_label();
        vector<string> get_features();        

    private:
        string name;
        string label;
        vector<string> features;
        node* tail;
        node* head;
};

#endif