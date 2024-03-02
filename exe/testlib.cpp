/* external headers */
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <gvc.h>

/* code mapper headers */
#include <cm_graph.h>
#include <cm_node.h>
#include <cm_edge.h>

using namespace std;

int main() 
{
    /* nodes and edges */    
    cm_node cm_exe("exe","exe/main.cpp");
    cm_node cm_gui("gui","gui/main.cpp");    
    cm_node cm_lib("lib","{lib/*|class node\\nclass edge\\nclass graph}");
    cm_lib.add_feature("shape = record");
    cm_node cm_lan("lan","{lan/*|source language}");
    cm_lan.add_feature("shape = record");
    cm_node qt_lib("qt","Qt5");
    qt_lib.add_feature("shape = box");
    qt_lib.add_feature("style = filled");
    qt_lib.add_feature("fillcolor = lightgray");    
    cm_node grphvz("graphviz","graphviz");
    grphvz.add_feature("style = filled");
    grphvz.add_feature("fillcolor = lightgray");
    cm_edge dep1("","",&cm_exe,&cm_lib);
    cm_edge dep2("","",&cm_gui,&cm_lib);
    cm_edge dep3("","",&cm_exe,&cm_lan);
    cm_edge dep4("","",&cm_gui,&cm_lan);
    cm_edge dep5("","",&cm_gui,&qt_lib);
    cm_edge dep6("","",&cm_exe,&grphvz);
    cm_edge dep7("","",&cm_gui,&grphvz);    
    
    /* graph building */
    cm_graph test("codemapper");
    test.addnode(&cm_exe);
    test.addnode(&cm_gui);
    test.addnode(&cm_lib);
    test.addnode(&cm_lan);
    test.addnode(&qt_lib);
    test.addnode(&grphvz);
    test.addedge(&dep1);
    test.addedge(&dep2);
    test.addedge(&dep3);
    test.addedge(&dep4);
    test.addedge(&dep5);
    test.addedge(&dep6);
    test.addedge(&dep7);
    test.edgesall("arrowhead = none");

    std::cout << test.to_string() << std::endl;

    return(0);
}

