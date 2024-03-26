#ifndef CM_EDGE_H
#define CM_EDGE_H

#include <string>
#include <vector>
#include <cm_node.h>

using namespace std;
class cm_node;
/*
 ___ ___   ___ ___ 
| __|   \ / __| __|
| _|| |) | (_ | _| 
|___|___/ \___|___|

*/               
class cm_edge 
{
    public:
        cm_edge(const string& in_name, const string& in_label, cm_node* tail, cm_node* head);
        void add_feature(const string& feature);
        cm_node* get_head();
        cm_node* get_tail();
        string get_name();
        string get_label();
        string humanreadable();
        vector<string> get_features();        

    private:
        string name;
        string label;
        vector<string> features;
        cm_node* tail;
        cm_node* head;
};

#endif