#ifndef CPP_LANGUAGE_H
#define CPP_LANGUAGE_H

#include <string>
#include <vector>
#include <cm_graph.h>
#include <cm_lan.h>

class cpp_language : public language
{
    public:
        cpp_language(const string& in_target);
        ~cpp_language() {};   
        void create_nodes(cm_graph* project) override;
        void create_edges(cm_graph* project) override;
        void mode_c_or_cpp(int mode);
};

#endif