#ifndef CM_NODE_H
#define CM_NODE_H

#include <string>
#include <vector>
#include <cm_edge.h>

using namespace std;
class edge;
/*
 _  _  ___  ___  ___ 
| \| |/ _ \|   \| __|
| .` | (_) | |) | _| 
|_|\_|\___/|___/|___|

*/                   
class cm_node 
{
    public:
        cm_node(const string& in_name, const string& in_label);
        void add_feature(const string& feature);
        string get_name();
        string get_label();
        vector<string> get_features();

    private:
        string name;
        string label;
        vector<string> features;
};

#endif