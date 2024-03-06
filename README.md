# Code Mapper

Code Mapper is a tool to generate standard ```graphviz``` graphs from source code.

## Requirements

Code Mapper sources must link with ```graphviz``` as a library, so first to compile  ```codemapper``` sources your system must find ```graphviz``` sources.

```
$ git clone https://gitlab.com/graphviz/graphviz
$ cd graphviz
$ mkdir build
$ cmake ..
$ make
$ make install
```

To complete this previous step, may be other packages could be required, as ```bison``` or ```flex```, etc.
```
$ sudo apt-get install bison
$ sudo apt-get install flex
```

## Codemapper separated graph lib

Inside ```lib``` folder the codemapper own graph lib is availble. To be used a ```cm_graph``` the ```exe/testlib.cpp``` is available.
```
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

    /* output */
    std::cout << test.to_string() << std::endl;
```
And used as follows:
```
./codemapper | ./simple > codemapper.png
```
![example](basic.png)