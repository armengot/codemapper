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
class graph
{
    public:
        graph(string in_name);
        ~graph(); 

        string to_string() const;
        void addnode(node* node);
        void addedge(edge* edge);

    private:
        string name;
        vector<node*> nodes;
        vector<edge*> edges;
};

#endif