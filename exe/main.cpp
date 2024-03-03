/* external headers */
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <gvc.h>

/* code mapper headers */
#include <cm_lan.h>
#include <cm_clan.h>
#include <cm_graph.h>
#include <cm_node.h>
#include <cm_edge.h>
#include <gitinfo.h>

using namespace std;

void cm_render(const string& input, std::stringstream& output) 
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

int main(int argc, char* argv[]) 
{
    string target_folder;
    string lang;
    string output_format = "png";
    bool target_provided = false;
    bool lang_provided = false;
    cm_graph* codetree;

    int opt;
    while ((opt = getopt(argc, argv, "t:l:v:o:")) != -1) 
    {
        switch (opt) {
            case 't':
                target_folder = optarg;
                target_provided = true;
                break;
            case 'l':
                lang = optarg;
                lang_provided = true;
                break;
            case 'v':
                cout << "codemapper by Marcelo Armengot (C) 2024 " << VERSION << endl;                
                return 0;
            case 'o':
                output_format = optarg;
                break;
            default:
                cout << "codemapper by Marcelo Armengot (C) 2024 " << VERSION << endl;                            
                cout << "Usage: " << argv[0] << " -t folder -l lang [-v] [-o output_format]" << endl;
                cout << "\t\t-t root folder of the target project" << endl;
                cout << "\t\t-l source code language of the target project (currenly only available \"py\" for Python or \"cpp\" for C/C++" << endl;
                cout << "\t\t-v version info" << endl;
                cout << "\t\t-o output format (svg/png)";
                return 1;
        }
    }
    
    if (!target_provided || !lang_provided) 
    {
        cout << "codemapper by Marcelo Armengot (C) 2024 " << VERSION << endl;                            
        cout << "Usage: " << argv[0] << " -t folder -l lang [-v] [-o output_format]" << endl;        
        cerr << "Missing mandatory parameters language and target folder." << std::endl;       
        return(1);
    }
    
    cout << "   Specified target: " << target_folder << endl;
    cout << "    Code written in: " << lang << endl;
    cout << "Output graph format: " << output_format << endl;

    if (lang[0]=='c')
    {
        cpp_language project(target_folder);
        codetree = project.parse();
        project.createnodes(codetree);
        codetree->to_string();
    }

    return(0);
}


/*
    stringstream output;    
    std::thread processingThread(cm_render, test.to_string(), std::ref(output));      
    processingThread.join();    
    std::string svg = output.str();    
    std::cout << svg << std::endl;
*/

