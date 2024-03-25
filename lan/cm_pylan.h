#ifndef PY_LANGUAGE_H
#define PY_LANGUAGE_H

#include <string>
#include <vector>
#include <cm_graph.h>
#include <cm_lan.h>

class py_language : public language
{
    public:
        py_language(const string& in_target);
        ~py_language() {};   
        void create_nodes(cm_graph* project) override;
        void create_edges(cm_graph* project) override;        
};

#endif