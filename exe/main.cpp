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

void render(const string& input, std::stringstream& output) 
{
    GVC_t *gvc;
    Agraph_t *g;
    FILE *fp;
    gvc = gvContext();
    fp = fmemopen((void*)input.c_str(), input.length(), "r");
    g = agread(fp, 0);
    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "png", stdout);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    fclose(fp);
    output << stdout;
}

int main() 
{
    cm_graph test("test");
    string example = "digraph { A [color = blue] A -> B; }";    
    stringstream output;    
    std::thread processingThread(render, example, std::ref(output));    
    processingThread.join();    
    std::string svg = output.str();    
    std::cout << svg << std::endl;
    return(0);
}
