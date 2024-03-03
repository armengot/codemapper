#ifndef CM_LAN_H
#define CM_LAN_H

#include <string>
#include <vector>
#include <cm_graph.h>

using namespace std;

class language 
{
    public:
        language(const string& in_target);
        ~language();
        cm_graph parse();
    
    protected:
        void lookfor_sources();
        void lookfor_deps();
        vector<cm_node> sources;
        vector<cm_edge> deps;
        string src_ext;
        string lib_ext;
        string inc_key;
        string target;
};

#endif
