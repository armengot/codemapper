#ifndef CM_GRAPH_H
#define CM_GRAPH_H

#include <string>
#include <vector>
#include <cm_edge.h>
#include <cm_node.h>

using namespace std;
/*
  ___ ___    _   ___ _  _ 
 / __| _ \  /_\ | _ \ || |
| (_ |   / / _ \|  _/ __ |
 \___|_|_\/_/ \_\_| |_||_|
    
*/                   
class cm_graph
{
    public:
        cm_graph(const string& in_name);
        ~cm_graph(); 

        string to_string() const;
        void addnode(cm_node* node);
        void addedge(cm_edge* edge);
        void edgesall(const string& feature);        

    private:
        string name;
        vector<cm_node*> nodes;
        vector<cm_edge*> edges;
};

#endif