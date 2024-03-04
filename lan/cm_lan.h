#ifndef CM_LAN_H
#define CM_LAN_H

/* external headers */
#include <map>
#include <string>
#include <vector>

/* codemapper header */
#include <cm_graph.h>

using namespace std;

/* abstract class */
class language 
{
    public:
        language(const string& in_target);
        ~language();
        cm_graph* parse();
        virtual void create_nodes(cm_graph* project) = 0;
        virtual void create_edges(cm_graph* project) = 0;
    
    protected:
        void lookfor_sources();
        void lookfor_deps();    
        vector<string> sourceslist;
        vector<cm_node> node_sources;
        vector<cm_edge> deps;
        map<string,string> sourcesmap;
        string src_ext;
        string lib_ext;
        string inc_key;
        string target;
};

#endif
