/* external headers */
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

/* code mapper headers */
#include <tools.h>
#include <cm_lan.h>
#include <cm_clan.h>
#include <cm_graph.h>
#include <cm_node.h>
#include <cm_edge.h>
#include <gitinfo.h>

using namespace std;

int main(int argc, char* argv[]) 
{
    string target_folder;
    string lang;
    string output_format = "png";
    bool target_provided = false;
    bool lang_provided = false;
    bool format_provided = false;
    cm_graph* codetree = nullptr;
    cpp_language* project = nullptr;

    int opt;
    while ((opt = getopt(argc, argv, "t:l:o:vh")) != -1) 
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
                cerr << "codemapper by Marcelo Armengot (C) 2024 " << GIT_OFFICIAL_VERSION << endl;
                cerr << "graphviz rendering has been included from version " << PACKAGE_VERSION << endl;                
                cerr << "more info in: " << CODEMAPPER_URL << endl;
                return 0;
            case 'o':
                output_format = optarg;
                break;
            case 'h':
                cerr << "codemapper by Marcelo Armengot (C) 2024 " << GIT_OFFICIAL_VERSION << endl;
                cerr << "graphviz rendering has been included from version " << PACKAGE_VERSION << endl;
                cerr << "more info in: " << CODEMAPPER_URL << endl << endl;
                cerr << "Usage: " << argv[0] << " -t folder -l lang [-v] [-o output_format]" << endl;
                cerr << "\t\t-t root folder of the target project." << endl;
                cerr << "\t\t-l source code language of the target project, currenly only available \"cpp\" for C/C++ (Python in progress)." << endl;
                cerr << "\t\t-v version info." << endl;
                cerr << "\t\t-o output format (svg/png/dot)." << endl;
                return 1;
        }
    }
    
    if (!target_provided || !lang_provided) 
    {
        cerr << "codemapper by Marcelo Armengot (C) 2024 " << GIT_OFFICIAL_VERSION << endl;
        cerr << "graphviz rendering has been included from version " << PACKAGE_VERSION << endl;
        cerr << "Usage: " << argv[0] << " -t folder -l lang [-v] [-o output_format] [h for help]" << endl;        
        cerr << "Missing mandatory parameters language and target folder." << std::endl;       
        return(1);
    }

    if (!format_provided)
    {
        cerr << "Missing output format, " << output_format << " default option selected." << endl;
    }
    
    cerr << "-------------------------------------------------------------------------------------------" << endl;
    cerr << "   Specified target: " << target_folder << endl;
    cerr << "    Code written in: " << lang << endl;
    cerr << "Output graph format: " << output_format << endl;
    cerr << "-------------------------------------------------------------------------------------------" << endl;

    if (lang[0]=='c')
    {
        project = new cpp_language(target_folder);
        if (lang == "cpp")
            project->mode_c_or_cpp(1); // C++
        else
            project->mode_c_or_cpp(0); // C
        codetree = project->parse();
        project->create_nodes(codetree);
        project->create_edges(codetree);
    }

    if ((output_format == "svg")||(output_format=="SVG"))
    {
        string output;
        cm_render(codetree->to_string(), output, CM_OUTPUT_SVG);        
        cout << output << endl;  
    }
    if ((output_format == "png")||(output_format=="PNG"))
    {
        string output;
        cm_render(codetree->to_string(), output, CM_OUTPUT_PNG);        
        cout << output << endl;  
    }    
    if ((output_format == "dot")||(output_format=="DOT"))
    {
        cout << codetree->to_string() << endl;        
    }        

    return(0);
}

