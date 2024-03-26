/* external headers */
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

/* codemapper headers */
#include <cm_graph.h>
#include <cm_node.h>
#include <cm_edge.h>
#include <tools.h>

/* graphviz headers */
#include <cgraph.h>

cm_graph::cm_graph(const std::string& in_name)
{
    name = in_name;
}

cm_graph::cm_graph(Agraph_t *g)
{    
    Agnode_t *n;
    Agedge_t *e;
    Agsym_t *sym; 
    cm_node* wnode;
    cm_edge* wedge;
    if (g == nullptr)
    {
        return;    
    }
    fprintf(stderr,"cm_graph(g): graphviz internal graph to be parsed as cm_grap class\n");
    fprintf(stderr,"cm_graph(g): running over graph called [%s]\n", agnameof(g));
    name.assign(agnameof(g));    
    for (n = agfstnode(g); n != NULL; n = agnxtnode(g, n)) 
    {
        fprintf(stderr,"cm_graph(g): Node: %s\n", agnameof(n));
        std::string wname = std::string(agnameof(n));
        wnode = new cm_node(wname,"");
        for (sym = agnxtattr(g, AGNODE, NULL); sym != NULL; sym = agnxtattr(g, AGNODE, sym)) 
        {
            std::string feature;
            fprintf(stderr,"cm_graph(g): Node feature: %s = %s\n", sym->name, agxget(n, sym));                        
            if (std::strcmp(sym->name, "label") == 0) 
            {
                feature = std::string(agxget(n,sym));
                wnode->set_label(feature);
            }
            else
            {
                if (std::string(agxget(n,sym))!="")                
                {
                    feature = std::string(sym->name) + " = " + std::string(agxget(n,sym));
                    char *q = agxget(n,sym);
                    size_t l = strlen(q);
                    if (l>0)
                        wnode->add_feature(feature);
                }
            }
            addnode(wnode);
        }               
    }    
    for (n = agfstnode(g); n != NULL; n = agnxtnode(g, n)) 
    {
        fprintf(stderr,"cm_graph(g): Node %s edges: \n", agnameof(n));
        for (e = agfstout(g, n); e != NULL; e = agnxtout(g, e)) 
        {
            cm_node* head = lookfor(std::string(agnameof(aghead(e))));
            cm_node* tail = lookfor(std::string(agnameof(agtail(e))));
            cm_edge* wedge = new cm_edge("","",tail,head);
            fprintf(stderr,"cm_graph(g): Edge: %s -> %s\n", agnameof(agtail(e)), agnameof(aghead(e)));
            for (sym = agnxtattr(g,AGEDGE,NULL); sym != NULL; sym = agnxtattr(g,AGEDGE,sym))
            {           
                bool insert = true;     
                std::string feature = std::string(sym->name) + " = " + std::string(agxget(e, sym));                
                char *q = agxget(e, sym);
                size_t l = strlen(q);
                if (l>0)
                {
                    //fprintf(stderr,"cm_graph: Edge feature: %s = %s [%ld]\n", sym->name, agxget(e, sym),l);                               
                    for (const auto& feature : wedge->get_features())
                    {
                        size_t pos = feature.find(std::string(sym->name));
                        if (pos==std::string::npos)
                        {
                            insert = false;
                        }
                    }
                    if (insert)
                        wedge->add_feature(feature);
                }
            }
            addedge(wedge);
        }
    }    
}

cm_graph::~cm_graph()
{   
    /*
    for (auto cm_edge : edges) 
    {
        delete cm_edge;
    }
    for (auto cm_node : nodes) 
    {
        delete cm_node;
    }
    */
}

void cm_graph::edgesall(const string& feature)
{
    for (const auto& edge : edges) 
    {
        edge->add_feature(feature);
    }      
}

int cm_graph::addnode(cm_node* node)
{
    for (auto& each_node : nodes)
    {        
        if (each_node->get_name()==node->get_name())
            return(0);
    }
    nodes.push_back(node);
    return(1);
}

int cm_graph::addedge(cm_edge* edge)
{
    cm_node* tail = edge->get_tail();
    cm_node* head = edge->get_head();

    if (tail==head)
        return(0);

    for (auto& each_edge : edges)
    {
        if ((tail == each_edge->get_tail())&&(head == each_edge->get_head()))
            return(0);
    }
    bool backdir_stillempty = true;
    for (auto& feat : edge->get_features())
    {
        std::string feature = static_cast<std::string>(feat);
        if ((feature == "dir=back")||(feature == "dir = back"))
        {
            backdir_stillempty = false;
        }
    }
    if (backdir_stillempty)
        edge->add_feature("dir=back");
    edges.push_back(edge);
    return(1);
}

vector<cm_node*> cm_graph::allnodes()
{
    return(nodes);
}

cm_edge* cm_graph::nextedge(cm_edge* current, int tailhead)
{
    cm_node* comparison;
    if (tailhead ==0)
        comparison = current->get_head();
    else
        comparison = current->get_tail();
    cm_edge* bereturned = nullptr;
    int full_rotation = 0;
    bool next = false;
    while (bereturned==nullptr)
    {
        for (int i = 0; i < edges.size(); i++)
        {
            cm_edge* each = edges[i];
            bool check = false;
            if (tailhead == 0)        
            {
                check = (comparison == each->get_head());
            }
            else
            {
                check = (comparison == each->get_tail());
            }
            if (check)
            {
                if (current == each)
                {
                    next = true;
                }
                else
                {
                    if (bereturned == nullptr)
                    {
                        bereturned = each;
                    }
                    if (next)                
                        return(each);
                }
            }
        }
        full_rotation = full_rotation + 1;
        if (full_rotation==2)
        {
            bereturned = edges[0];
        }
    }
    return(bereturned);
}

void cm_graph::removeedge(cm_edge* edge)
{
    for (int i = edges.size() - 1; i >= 0; --i)
    {
        const cm_edge* each = edges[i];        
        if (edge == each)
        {
            edges.erase(edges.begin() + i);
        }
    }
}

void cm_graph::removenode(string name)
{
    cm_node* toberemoved = lookfor(name);
    if (toberemoved)
    {        
        for (auto it = edges.begin(); it != edges.end(); )
        {
            cm_edge* edge = *it;
            if (edge->get_head() == toberemoved || edge->get_tail() == toberemoved)
            {
                std::cerr << "removenode: Delete edge " << edge->get_head()->get_name() << " --> "  << edge->get_tail()->get_name() << std::endl;                
                edges.erase(it);
                delete edge;                
            }
            else
            {
                ++it;
            }
        }        
        auto it_node = std::find(nodes.begin(), nodes.end(), toberemoved);
        if (it_node != nodes.end())
        {
            cm_node* toremove = *it_node;
            std::cerr << "removenode: " << "Remove node " << toremove->get_name() << std::endl;
            nodes.erase(it_node);                        
            delete toremove;
        }
        std::cerr << "removenode: Node " << name << " removed successfully." << std::endl;
    }
    else
    {
        std::cerr << "removenode: Couldn't find node called " << DEBUG_MGTTXT << name << DEBUG_RESTXT;
    }
}

void cm_graph::reset_edge_colors()
{
    for(auto& edge : edges)
    {
        edge->erasecolor();
    }
}

cm_node* cm_graph::lookfor(std::string name)
{
    std::string copy;
    if (charin('.',name))        
    {
        copy = firstname(name,'.');
    }
    else
    {
        copy = name;
    }    
    if (charin(CM_SYS_SPLITER_CHAR,copy))
    {
        rechar(copy,CM_SYS_SPLITER_CHAR,CM_GLOBAL_JOIN_CHAR);
    }
    std::cerr << "cm_graph::lookfor: looking for " << copy << std::endl;    
    for (const auto& node : nodes) 
    {        
        //std::cerr << "cm_graph: comparing " << copy << " with " << node->get_name() << std::endl;
        if (copy == node->get_name())
        {
            std::cerr << "cm_graph::lookfor: Found node [" << node->get_name() << " : " << node->get_label() << "]" << std::endl;
            return(node);
        }
    }
    return(nullptr);
}

std::string cm_graph::to_string() const 
{
    std::stringstream ss;
    ss << "digraph " << name << "\n{\n";

    // write nodes
    for (const auto& node : nodes) 
    {
        ss << "\t" << node->get_name() << "\n\t[\n";
        ss << "\t\tlabel=\"" << node->get_label() << "\"\n";
        
        std::vector<std::string> features = node->get_features();
        for (const auto& feature : features) 
        {
            ss << "\t\t" << feature << "\n";
        }
        
        ss << "\t];\n";
    }
    
    // write edges
    for (const auto& edge : edges) 
    {
        ss << "\t" << edge->get_tail()->get_name() << " -> " 
           << edge->get_head()->get_name() << "\n\t[\n";
        ss << "\t\tlabel=\"" << edge->get_label() << "\"\n";
        
        std::vector<std::string> features = edge->get_features();
        for (const auto& feature : features) 
        {
            ss << "\t\t" << feature << "\n";
        }
        
        ss << "\t];\n";
    }

    ss << "}\n";
    return ss.str();
}

std::vector<cm_edge*> cm_graph::edgesinvolved(cm_node* node)
{
    std::vector<cm_edge*> rlist;
    cm_node* gtail;
    cm_node* ghead;
    
    for (auto& each_edge : edges)
    {
        gtail = each_edge->get_tail();
        ghead = each_edge->get_head();
        if ((node == gtail)||(node == ghead))
        {
            rlist.push_back(each_edge);
        }
    }    
    return(rlist);
}