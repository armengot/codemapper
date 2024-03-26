#ifndef CM_GRAPH_H
#define CM_GRAPH_H

/* standard external libraries */
#include <string>
#include <vector>

/* graphviz libraries */
#include <gvc.h>

/* codemapper libraries */
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
        //Agraph_t *g
        cm_graph(const string& in_name);
        cm_graph(Agraph_t *g);
        ~cm_graph(); 

        string to_string() const;
        int addnode(cm_node* node);
        int addedge(cm_edge* edge);
        void edgesall(const string& feature);
        cm_node* lookfor(string name);
        vector<cm_edge*> edgesinvolved(cm_node* node);
        vector<cm_node*> allnodes();
        void removenode(string name);
        void removeedge(cm_edge* edge);
        void reset_edge_colors();
        cm_edge* nextedge(cm_edge* current);

    private:
        string name;
        vector<cm_node*> nodes;
        vector<cm_edge*> edges;
};

#endif